#ifndef _SEND_EMAIL_H_
#define _SEND_EMAIL_H_

#include <sys/types.h>
//#include <sys/socket.h>
//#include <unistd.h>
//#include <netdb.h>
#include "ZBase64.h"
#include <string.h>
#include <iostream>
#include <Ws2tcpip.h>
#include <winsock2.h>


using namespace std;
namespace semail
{

	class SendEmail
	{
	public:
		SendEmail(const string& user = "", const string& password = "");
		~SendEmail();

		void send_email(const string& to_user, const string& title, const string& text);

		void get_socket(int& fd);

	private:
		int m_send();
		int m_recv();


		void get_sockaddr();
		void start_socket();

		void login_smtp();

		void quit_smtp();
	private:
		bool m_error;

		string m_user;
		string m_password;
		//string m_url;

		sockaddr_in m_servaddr;
		int m_sockfd;
		WSADATA wsaData;
		string m_send_buf;
		string m_recv_buf;
	};

}
#endif