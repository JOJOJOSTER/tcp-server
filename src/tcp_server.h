#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class TCP_Server {
public:
  explicit TCP_Server(int port = 54001, int maxClient = 3);

  ~TCP_Server();

  void PrintConnectedClient() const;

  bool GetHasError() const;

  void Setup_Server_Address(int port);

  // SOCKET
  void Init_Socket();

  // BIND
  void Bind_Socket();

  // LISTEN
  void Listen_Socket(int maxUser);

  // ACCEPT
  void Accept_Socket();

  // RECIEVE
  int Receive_Socket(std::string &recieve_message);

  void Send_Socket(const std::string &send_message);

private:
  // Server
  int server_fd;
  sockaddr_in serverAddress;
  // Client
  int temp_client_fd;
  sockaddr_in temp_client_address;
  socklen_t clientSize;

  //
  int m_maxClient;
  bool hasError;
};
