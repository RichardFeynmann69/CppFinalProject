#include "SocketClient.h"
#include <iostream>

// Initialize Winsock only on Windows
#ifdef _WIN32
void initializeSockets() {
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
}

void cleanupSockets() {
    WSACleanup();
}
#else
void initializeSockets() {
    // No initialization needed for Linux
}

void cleanupSockets() {
    // No cleanup needed for Linux
}
#endif

SocketClient::SocketClient(const std::string& ip_address, int port)
    : ip_address(ip_address), port(port), client_socket(INVALID_SOCKET) {
    initializeSockets();

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr);
}

SocketClient::~SocketClient() {
    closesocket(client_socket);
    cleanupSockets();
}

bool SocketClient::connectToServer() {
    int result = connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        return false;
    }
    return true;
}

bool SocketClient::sendFrame(const std::vector<unsigned char>& frame) {
    int result = send(client_socket, reinterpret_cast<const char*>(frame.data()), frame.size(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Send failed" << std::endl;
        return false;
    }
    return true;
}

void SocketClient::closeConnection() {
    closesocket(client_socket);
    client_socket = INVALID_SOCKET;
}

std::vector<unsigned char> createFrame(int row, const std::string& text) {
    unsigned char start_byte = 0xFF;
    unsigned char row_byte = static_cast<unsigned char>(row);
    std::vector<unsigned char> text_bytes(text.begin(), text.end());
    unsigned char nbytes = static_cast<unsigned char>(text_bytes.size());
    unsigned char end_byte = 0x00;

    std::vector<unsigned char> frame = { start_byte, row_byte, nbytes };
    frame.insert(frame.end(), text_bytes.begin(), text_bytes.end());
    frame.push_back(end_byte);

    return frame;
}