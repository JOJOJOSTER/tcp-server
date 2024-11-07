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

class ERROR_HANDLER {
private:
  std::string m_msg;
  bool m_flag;

public:
  ERROR_HANDLER() = default;

  ERROR_HANDLER(bool successfully, const std::string &msg)
      : m_flag(successfully), m_msg(msg){};

  bool isSuccesful() { return m_flag; }
  const std::string Get_Message() { return m_msg; }
};

class TCP_Server {
public:
  explicit TCP_Server(int port = 54001, int maxClient = 3);

  ~TCP_Server();

  void PrintConnectedClient() const;

  bool GetHasError() const;

  // SOCKET
  ERROR_HANDLER Init_Socket();

  // BIND
  ERROR_HANDLER Bind_Socket(int port);

  // LISTEN
  ERROR_HANDLER Listen_Socket(int maxUser);

  // ACCEPT
  ERROR_HANDLER Accept_Socket();

  ERROR_HANDLER Start(int port, int maxClient);

  // RECIEVE
  ERROR_HANDLER Receive_Socket(std::string &recieve_message);

  ERROR_HANDLER Send_Socket(const std::string &send_message);

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
