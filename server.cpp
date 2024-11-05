#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class TCP_Server {
public:
  explicit TCP_Server(int port = 54001, int maxClient = 1) { hasError = false; }

  /*
  void Server_Start(int port, int maxClient) {
    Init_Socket();
    Bind_Socket(port);
    Listen_Socket(maxClient);
    Accept_Socket();
  }
  */

  void PrintConnectedClient() const {

    if (hasError) {
      std::cout << "No connection" << std::endl;
    }

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    int result = getnameinfo((sockaddr *)&clientAddress, sizeof(clientAddress),
                             host, NI_MAXHOST, serv, NI_MAXSERV, 0);
    if (result) {
      std::cout << host << " connected on " << serv << std::endl;
    } else {
      inet_ntop(AF_INET, &clientAddress.sin_addr, host, NI_MAXHOST);
      std::cout << host << " connected on " << ntohs(clientAddress.sin_port)
                << std::endl;
    }
  }

  ~TCP_Server() {
    close(fdServer);
    close(fdClient);
  }

  bool GetHasError() const { return hasError; }

  // SOCKET
  void Init_Socket() {
    fdServer = socket(AF_INET, SOCK_STREAM, 0);
    if (fdServer == -1) {

      std::cerr << "Socket error" << std::endl;
      hasError = true;
    }
  }

  // BIND
  void Bind_Socket(int port) {

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(fdServer, (sockaddr *)&serverAddress, sizeof(serverAddress)) ==
        -1) {
      std::cerr << "Bind error" << std::endl;
      hasError = true;
    }
  }

  // LISTEN
  void Listen_Socket(int maxUser) {
    if (listen(fdServer, 5) == -1) {
      std::cerr << "Listen error" << std::endl;
      hasError = true;
    }
  }

  // ACCEPT
  void Accept_Socket() {
    if (accept(fdServer, (sockaddr *)&clientAddress, &clientSize) == -1) {
      std::cerr << "Accept error" << std::endl;
      hasError = true;
    }
  }

private:
  // Server
  int fdServer;
  sockaddr_in serverAddress;
  // Client
  int fdClient;
  sockaddr_in clientAddress;
  socklen_t clientSize;

  //
  int m_maxClient;
  bool hasError;
};

int main() {

  using namespace std;

  TCP_Server tcp_server;

  tcp_server.Init_Socket();

  if (tcp_server.GetHasError()) {
    return -1;
  }

  tcp_server.Bind_Socket(54001);

  if (tcp_server.GetHasError()) {
    return -2;
  }

  tcp_server.Listen_Socket(5);

  if (tcp_server.GetHasError()) {
    return -3;
  }

  tcp_server.Accept_Socket();

  if (tcp_server.GetHasError()) {
    return -4;
  }

  tcp_server.PrintConnectedClient();

  return 0;

  /*

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    // clean mem from garbage
    memset(host, 0, NI_MAXHOST);
    memset(serv, 0, NI_MAXSERV);

    if (result) {
      cout << host << " connected on " << serv << endl;
    } else {
      inet_ntop(AF_INET, &clientAddress.sin_addr, host, NI_MAXHOST);
      cout << host << " connected on " << ntohs(clientAddress.sin_port) << endl;
    }

    char buff[4096];

    while (true) std::{
      // RECEIVE
      int bytesRecv = recv(clientSocket, buff, 4096, 0);

      if (bytesRecv == -1) {
        cerr << "Connection issue" << endl;
        break;
      }

      if (bytesRecv == 0) {
        cerr << "Client disconnected" << endl;
        break;
      }

      cout << "Recieved: " << buff << endl;

      send(clientSocket, buff, bytesRecv, 0);
      memset(buff, 0, 4096);
    }

    close(clientSocket);

    return 0;
    */
}
