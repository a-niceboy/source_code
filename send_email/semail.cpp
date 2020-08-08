#include "semail.h"

namespace semail
{
	
class LogSuccess
{
public:
	void operator<<(const string& data)
	{
		string show = "\033[1;32m====  " + data + "  ====\033[0m";
		cout << show << endl;		
	}
};

class LogError
{
public:
	void operator<<(const string& data)
	{
		string show = "\033[0;31m####  " + data + "  ####\033[0m";
		cout << show << endl;	
	}
};

LogSuccess lg_ss;
LogError lg_er;

SendEmail::SendEmail(const string& user, const string& password, const string& smtp_url)
	: m_error(false)
	, m_user(user)
	, m_password(password)
	, m_url(smtp_url)
{	
	bzero(&m_servaddr,sizeof(m_servaddr));
	
	lg_ss << "email start";
	
	if(!m_error)
	{
		get_sockaddr();
	}
	
	if(!m_error)
	{
		start_socket();
	}
	
	if(!m_error)
	{
		login_smtp();
	}
}

SendEmail::~SendEmail()
{
	if(m_error)
	{
		lg_er << "email error";
	}
	else
	{
		lg_ss << "email success";
	}
	close(m_sockfd);
	lg_ss << "email destory ok";
}

void SendEmail::user(const string& user)
{
	m_user = user;
}
void SendEmail::password(const string& password)
{
	m_password = password;
}

void SendEmail::smtp_url(const string& smtp_url)
{
	m_url = smtp_url;
}

void SendEmail::get_socket(int& fd)
{
	fd = m_sockfd;
}

int SendEmail::m_send()
{
    size_t send_length = 0;
    while (send_length < m_send_buf.size())
    {
        int data_length = send(m_sockfd, m_send_buf.c_str() + send_length, m_send_buf.size() - send_length, 0);
        if (data_length < 0)
        {
            lg_er << "error send";
            close(m_sockfd);
            m_error = true;
            return -1;
        }

        send_length += data_length;
    }
    return send_length;
}

int SendEmail::m_recv()
{
    m_recv_buf = "";

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(m_sockfd, &fds);

    timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0L;

    int retval = select(m_sockfd + 1, &fds, NULL, NULL, &tv);

    if (retval < 0)
    {
        lg_er << "select error";
        m_error = true;
        return -1;
    }
    else if (retval == 0)
    {
        lg_er << "no data in the 20 second";
        m_error = true;
        return -3;
    }

    char rec[1500];
    memset(rec, 0, 1500);

    int data_length = recv(m_sockfd, rec, 1500, 0);

    if (data_length == 0)
    {
        lg_er << "peer closed";

    }
    else if (data_length < 0)
    {
        lg_er << "read error";
        m_error = true;
        return -2;
    }

    m_recv_buf = rec;

    return data_length;
}

void SendEmail::get_sockaddr()
{
    hostent* ph = gethostbyname(m_url.c_str());
    if (ph == NULL)
    {
        lg_er << "!!! no host";
        m_error = true;
        return;
    }

    m_servaddr.sin_family = AF_INET;
    m_servaddr.sin_port = htons(25);
    memcpy(&m_servaddr.sin_addr.s_addr, ph->h_addr_list[0], ph->h_length);
}

void SendEmail::start_socket()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0)
    {
        close(m_sockfd);
        lg_er << "start sockfd error";
        m_error = true;
        return;
    }
    if (connect(m_sockfd, (sockaddr*)&m_servaddr, sizeof(struct sockaddr_in)) < 0)
    {
        close(m_sockfd);
        lg_er << "connect sockfd error";
        m_error = true;
        return;
    }
}

void SendEmail::login_smtp()
{
    m_recv();
    if (m_recv_buf.find("220") == string::npos)
    {
        lg_er << "recv error";
        m_error = true;
        return;
    }
    
	// EHLO
    m_send_buf = "EHLO wy\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("250") == string::npos)
    {
        lg_er << "ehlo error";
        m_error = true;
        return;
    }

    //AUTH LOGIN
    m_send_buf = "AUTH LOGIN\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("334") == string::npos)
    {
        lg_er << "auth lohin error";
        m_error = true;
        return;
    }
    
    ZBase64 z64;
    m_send_buf = z64.Encode((const unsigned char*)m_user.c_str(), m_user.size());
    //base64(login, buf, strlen(buf));
    m_send_buf += "\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("334") == string::npos)
    {
        lg_er << "USER error";
        m_error = true;
        return;
    }

    // PASSWORD       
    m_send_buf = z64.Encode((const unsigned char*)m_password.c_str(), m_password.size());
    m_send_buf += "\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("235") == string::npos)
    {
        lg_er << "password error";
		m_error = true;
        return;
    }
}

void SendEmail::send_email(const string& to_user, const string& title, const string& text)
{
	if(m_error)
	{
		return;
	}
	lg_ss << "send mail";    
    // MAIL FROM
    m_send_buf = "MAIL FROM: <";
    m_send_buf = m_send_buf + m_user + ">\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("250") == string::npos)
    {
        lg_er << "mail from error";
        m_error = true;
        return;
    }
    
    // RCPT TO
    m_send_buf = "RCPT TO:<";
    m_send_buf = m_send_buf + to_user + ">\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("250") == string::npos)
    {
        lg_er << "rcpt to error";
        m_error = true;
        return;
    }

    // DATA
    m_send_buf = "DATA\r\n";
    m_send();
    m_recv();
    if (m_recv_buf.find("354") == string::npos)
    {
        lg_er << "data error";
        m_error = true;
        return;
    }

    // ·¢ËÍÓÊ¼þÄÚÈÝ
    m_send_buf = "From:<";
    m_send_buf = m_send_buf + m_user + ">\r\n"
        "To:<" + to_user + ">\r\n"
        "Subject: " + title + "\r\n\r\n"
        + text + "\r\n.\r\n";

    m_send();
    m_recv();
    if (m_recv_buf.find("250") == string::npos)
    {
        lg_er << "send error";
        m_error = true;
        return;
    }
    quit_smtp();
}

void SendEmail::quit_smtp()
{
    m_send_buf = "QUIT\r\n";
    m_send();
    m_recv();
}

}