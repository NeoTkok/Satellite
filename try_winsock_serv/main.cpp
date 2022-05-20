#include <SDKDDKVer.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")
#include "handle_connections.h"
#pragma warning(disable: 4996)


// std::map<int, std::map<int, std::string>> ID_Type;
/** SAT and CLIENT contain ID and SOCKET of units **/


int main(int argc, char* argv[]) {
    //WSAStartup
    WSAData wsaData{};
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = inet_addr("192.168.3.4");
    addr.sin_port = htons(1111);   //port 1111 connection
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sListen, (SOCKADDR *) &addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    NewClients(sListen, addr); //handle clients file

    return 0;
}