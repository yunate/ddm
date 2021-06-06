
#include "../test/Test_Socket.h"

int main()
{
    WSADATA wsaData = { 0 };
    int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult)
    {

    }

    // Test_Tcp_Server();
    // Test_Tcp_Client();
    // Test_Udp_Server();
    // Test_Udp_Client();
    // Test_Http_Get();
    Test_Http_Download();
    ::WSACleanup();
    ::system("pause");
    return 1;
}
