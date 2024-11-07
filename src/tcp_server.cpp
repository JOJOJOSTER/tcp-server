#include "tcp_server.h"
#include <cerrno>
#include <cstring>

TCP_Server::TCP_Server(int port, int maxClient) : hasError(false) {}

TCP_Server::~TCP_Server() {
  close(server_fd);
  close(temp_client_fd);
}

void TCP_Server::PrintConnectedClient() const {

  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];

  /*
    int result =
        getnameinfo((sockaddr *)&temp_client_address,
    sizeof(temp_client_address), host, NI_MAXHOST, serv, NI_MAXSERV, 0);

    if (result) {

      std::cout << host << " connected on " << serv << std::endl;

    } else {
    }
    */
  inet_ntop(AF_INET, &temp_client_address.sin_addr, host, NI_MAXHOST);
  std::cout << host << " connected on " << ntohs(temp_client_address.sin_port)
            << std::endl;
}

bool TCP_Server::GetHasError() const { return hasError; }

// SOCKET
ERROR_HANDLER TCP_Server::Init_Socket() {

  std::cout << "[SERVER]> Initializing socket..." << std::endl;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {

    return ERROR_HANDLER(false, strerror(errno));
  }

  return ERROR_HANDLER(true, strerror(errno));
}

// BIND
ERROR_HANDLER TCP_Server::Bind_Socket(int port) {

  std::cout << "[SERVER]> Binding socket" << std::endl;

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  int bind_ret =
      bind(server_fd, (sockaddr *)&serverAddress, sizeof(serverAddress));

  if (bind_ret < 0) {
    return ERROR_HANDLER(false, strerror(errno));
  }

  return ERROR_HANDLER(true, strerror(errno));
}

// LISTEN
ERROR_HANDLER TCP_Server::Listen_Socket(int maxUser) {

  std::cout << "[SERVER]> Listening socket..." << std::endl;

  int listen_ret = listen(server_fd, 5);

  if (listen_ret < 0) {
    return ERROR_HANDLER(false, strerror(errno));
  }

  return ERROR_HANDLER(true, strerror(errno));
}

// ACCEPT
ERROR_HANDLER TCP_Server::Accept_Socket() {

  std::cout << "[SERVER]> Handling new connection..." << std::endl;

  clientSize = sizeof(temp_client_address);
  temp_client_fd =
      accept(server_fd, (sockaddr *)&temp_client_address, &clientSize);

  if (temp_client_fd < 0) {
    return ERROR_HANDLER(false, strerror(errno));
  }

  return ERROR_HANDLER(true, strerror(errno));
}

ERROR_HANDLER TCP_Server::Start(int port, int maxClients) {
  ERROR_HANDLER ret_error;
  ret_error = Init_Socket();
  if (!ret_error.isSuccesful())
    return ret_error;

  ret_error = Bind_Socket(port);
  if (!ret_error.isSuccesful())
    return ret_error;

  ret_error = Listen_Socket(maxClients);
  if (!ret_error.isSuccesful())
    return ret_error;

  return ERROR_HANDLER(true, "Server started...");
}

ERROR_HANDLER TCP_Server::Receive_Socket(std::string &recieve_message) {
  char buff[4096];

  memset(buff, 0, 4096);

  int bytes_received = recv(temp_client_fd, buff, 4096, 0);

  if (bytes_received == 0) {
    return ERROR_HANDLER(false, "Client disconnect");
  }

  if (bytes_received < 0) {
    return ERROR_HANDLER(false, strerror(errno));
  }

  recieve_message = buff;
  return ERROR_HANDLER(true, "Data recieved");
}

ERROR_HANDLER TCP_Server::Send_Socket(const std::string &send_message) {

  int send_ret =
      send(temp_client_fd, send_message.c_str(), send_message.size(), 0);

  if (send_ret < 0) {
    return ERROR_HANDLER(false, strerror(errno));
  }

  return ERROR_HANDLER(true, "Data sended");
}
