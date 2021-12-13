#include <iostream>

#ifdef __linux__

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>  // for memset
#include <sys/socket.h>
#include <unistd.h>  // for close

#elif _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#endif

struct TcpServer {
  TcpServer(unsigned short port) {
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
#endif
    sockaddr_in srcAddr;
    memset(&srcAddr, 0, sizeof(srcAddr));
    srcAddr.sin_port = htons(port);
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(m_socket, (sockaddr *)&srcAddr, sizeof(srcAddr));
    listen(m_socket, 1);
  }

  ~TcpServer() {
#ifdef _WIN32
    WSACleanup();
#endif
  }

  void run() {
    int client;
    sockaddr_in dstAddr;
    socklen_t dstAddrSize = sizeof(dstAddr);

    while (1) {
      printf("Waiting for connection ...\n");
      client = accept(m_socket, (sockaddr *)&dstAddr, &dstAddrSize);
      printf("Connected from %s\n", inet_ntoa(dstAddr.sin_addr));

      int numRecv = 0;
      char buffer[512];
      while ((numRecv = recv(client, buffer, 512, 0)) > 0) {
        printf("received: %s\n", buffer);
      }
#ifdef __linux__
      close(client);
#elif _WIN32
      closesocket(client);
#endif
    }
  }

  int m_socket;
};

int main() {
  TcpServer server(9876);
  server.run();
}
