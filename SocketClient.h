#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <string>
#include <vector>

// Include necessary headers based on the platform
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
typedef int SOCKET;
#endif

class SocketClient {
public:
    SocketClient(const std::string& ip_address, int port);
    ~SocketClient();
    bool connectToServer();
    bool sendFrame(const std::vector<unsigned char>& frame);
    void closeConnection();

private:
    std::string ip_address;
    int port;
    SOCKET client_socket;
    sockaddr_in server_addr;
};

std::vector<unsigned char> createFrame(int row, const std::string& text);

#endif // SOCKETCLIENT_H