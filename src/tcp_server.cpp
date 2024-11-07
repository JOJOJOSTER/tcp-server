#include "tcp_server.h"

TCP_Server::TCP_Server(int port, int maxClient) : hasError(false) {

  Setup_Server_Address(port);
}

TCP_Server::~TCP_Server() {
  close(server_fd);
  close(temp_client_fd);
}

void TCP_Server::PrintConnectedClient() const {

  if (hasError) {
    std::cout << "No connection" << std::endl;
    return;
  }

  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];

  int result =
      getnameinfo((sockaddr *)&temp_client_address, sizeof(temp_client_address),
                  host, NI_MAXHOST, serv, NI_MAXSERV, 0);

  if (result) {

    std::cout << host << " connected on " << serv << std::endl;

  } else {

    inet_ntop(AF_INET, &temp_client_address.sin_addr, host, NI_MAXHOST);
    std::cout << host << " connected on " << ntohs(temp_client_address.sin_port)
              << std::endl;
  }
}

bool TCP_Server::GetHasError() const { return hasError; }

void TCP_Server::Setup_Server_Address(int port) {
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);
}

// SOCKET
void TCP_Server::Init_Socket() {

  std::cout << "[SERVER]> Initializing socket..." << std::endl;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {

    std::cerr << "[SERVER] [ERROR] Socket error" << std::endl;
    hasError = true;
  }
}

// BIND
void TCP_Server::Bind_Socket() {

  std::cout << "[SERVER]> Binding socket" << std::endl;

  int bind_ret =
      bind(server_fd, (sockaddr *)&serverAddress, sizeof(serverAddress));

  if (bind_ret < 0) {
    std::cerr << "[SERVER] [ERROR]> Bind error..." << std::endl;
    hasError = true;
  }
}

// LISTEN
void TCP_Server::Listen_Socket(int maxUser) {

  std::cout << "[SERVER]> Listening socket..." << std::endl;

  int listen_ret = listen(server_fd, 5);

  if (listen_ret < 0) {
    std::cerr << "[SERVER] [ERROR]> Listen error..." << std::endl;
    hasError = true;
  }
}

// ACCEPT
void TCP_Server::Accept_Socket() {

  std::cout << "[SERVER]> Handling new connection..." << std::endl;

  clientSize = sizeof(temp_client_address);
  temp_client_fd =
      accept(server_fd, (sockaddr *)&temp_client_address, &clientSize);

  if (temp_client_fd < 0) {
    std::cerr << "[SERVER] [ERROR]> Accept error..." << std::endl;
    hasError = true;
  }
}

int TCP_Server::Receive_Socket(std::string &recieve_message) {
  char buff[4096];

  memset(buff, 0, 4096);

  int bytes_received = recv(temp_client_fd, buff, 4096, 0);

  if (bytes_received == 0) {
    std::cerr << "[SERVER] Connection off" << std::endl;
    return 0;
  }

  if (bytes_received < 0) {
    std::cerr << "[SERVER] [ERROR]> Recieve error" << std::endl;
    hasError = true;
    return -1;
  }

  recieve_message = buff;
  return 1;
}

void TCP_Server::Send_Socket(const std::string &send_message) {

  int send_ret =
      send(temp_client_fd, send_message.c_str(), send_message.size(), 0);

  std::cout << "[SERVER]> Sending message..." << std::endl;

  if (send_ret < 0) {
    std::cerr << "[SERVER] [ERROR]> Send error" << std::endl;
  }
}
