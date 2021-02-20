#include <iostream>
#include "Semail.h"
using namespace std;
using namespace semail;

#pragma comment(lib,"ws2_32.lib")
int main()
{
    SendEmail se("157202207@qq.com", "klpbtybtuzmgcabf");
    se.send_email("897389307@qq.com", "title", "data");
}
