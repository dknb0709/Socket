#include <iostream>

#ifdef __linux__

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#elif _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#endif

struct TcpClient {
  TcpClient(const char* destination, unsigned short port) {
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
#endif

    sockaddr_in dstAddr;
    memset(&dstAddr, 0, sizeof(dstAddr));
    dstAddr.sin_port = htons(port);
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_addr.s_addr = inet_addr(destination);

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("Trying to connect to %s: \n", destination);
    connect(m_socket, (struct sockaddr*)&dstAddr, sizeof(dstAddr));
  }

  ~TcpClient() {
#ifdef __linux__
    close(m_socket);
#elif _WIN32
    closesocket(m_socket);
    WSACleanup();
#endif
  }

  void send(const char* data, int len) { ::send(m_socket, data, len, 0); }

  int m_socket;
};

int main() {
  TcpClient client("127.0.0.1", 9876);
  for (int i = 0; i < 5; ++i) {
    printf("sending...\n");
    client.send("this is a Test", 15);
#ifdef __linux__
    sleep(1);
#elif _WIN32
    Sleep(1000);
#endif
  }
}
