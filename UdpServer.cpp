#include <iostream>

#ifdef __linux__

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#elif _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#endif

struct UdpServer {
  UdpServer(unsigned short port) {
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
#endif

    sockaddr_in recvSockAddr;
    memset(&recvSockAddr, 0, sizeof(recvSockAddr));
    recvSockAddr.sin_port = htons(port);
    recvSockAddr.sin_family = AF_INET;
    recvSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    bind(m_socket, (sockaddr*)&recvSockAddr, sizeof(recvSockAddr));
  }

  ~UdpServer() {
#ifdef _WIN32
    WSACleanup();
#endif
  }

  void run() {
    char buffer[512];
    while (1) {
      int numrcv = recvfrom(m_socket, buffer, 512, 0, NULL, NULL);
      if (numrcv == -1) {
#ifdef __linux__
        close(m_socket);
#elif _WIN32
        closesocket(m_socket);
#endif
        break;
      }
      printf("received: %s\n", buffer);
    }
  }

  int m_socket;
};

int main() {
  UdpServer server(9876);
  server.run();
}
