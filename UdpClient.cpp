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

struct UdpClient {
  UdpClient(const char* destination, unsigned short port) {
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 0), &data);
#endif

    memset(&m_destSockAddr, 0, sizeof(m_destSockAddr));
    m_destSockAddr.sin_addr.s_addr = inet_addr(destination);
    m_destSockAddr.sin_port = htons(port);
    m_destSockAddr.sin_family = AF_INET;

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
  }

  ~UdpClient() {
#ifdef __linux__
    close(m_socket);
#elif _WIN32
    closesocket(m_socket);
    WSACleanup();
#endif
  }

  void send(const char* data, int len) {
    sendto(m_socket, data, len, 0, (sockaddr*)&m_destSockAddr,
           sizeof(m_destSockAddr));
  }

  int m_socket;
  sockaddr_in m_destSockAddr;
};

int main() {
  UdpClient client("127.0.0.1", 9876);
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
