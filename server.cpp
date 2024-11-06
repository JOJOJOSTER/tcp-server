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
  explicit TCP_Server(int port = 54001, int maxClient = 1) : hasError(false) {

    Setup_Server_Address(port);
  }

  ~TCP_Server() {
    close(server_fd);
    close(temp_client_fd);
  }

  void PrintConnectedClient() const {

    if (hasError) {
      std::cout << "No connection" << std::endl;
      return;
    }

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    int result = getnameinfo((sockaddr *)&temp_client_address,
                             sizeof(temp_client_address), host, NI_MAXHOST,
                             serv, NI_MAXSERV, 0);

    if (result) {

      std::cout << host << " connected on " << serv << std::endl;

    } else {

      inet_ntop(AF_INET, &temp_client_address.sin_addr, host, NI_MAXHOST);
      std::cout << host << " connected on "
                << ntohs(temp_client_address.sin_port) << std::endl;
    }
  }

  bool GetHasError() const { return hasError; }

  void Setup_Server_Address(int port) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
  }

  // SOCKET
  void Init_Socket() {

    std::cout << "[SERVER]> Initializing socket..." << std::endl;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {

      std::cerr << "[SERVER] [ERROR] Socket error" << std::endl;
      hasError = true;
    }
  }

  // BIND
  void Bind_Socket() {

    std::cout << "[SERVER]> Binding socket" << std::endl;

    int bind_ret =
        bind(server_fd, (sockaddr *)&serverAddress, sizeof(serverAddress));

    if (bind_ret < 0) {
      std::cerr << "[SERVER] [ERROR]> Bind error..." << std::endl;
      hasError = true;
    }
  }

  // LISTEN
  void Listen_Socket(int maxUser) {

    std::cout << "[SERVER]> Listening socket..." << std::endl;

    int listen_ret = listen(server_fd, 5);

    if (listen_ret < 0) {
      std::cerr << "[SERVER] [ERROR]> Listen error..." << std::endl;
      hasError = true;
    }
  }

  // ACCEPT
  void Accept_Socket() {

    clientSize = sizeof(temp_client_address);
    temp_client_fd =
        accept(server_fd, (sockaddr *)&temp_client_address, &clientSize);

    if (temp_client_fd < 0) {
      std::cerr << "[SERVER] [ERROR]> Accept error..." << std::endl;
      hasError = true;
    }
  }

  void Receive_Socket(std::string &recieve_message) {
    char buff[4096];

    memset(buff, 0, 4096);

    int bytes_received = recv(temp_client_fd, buff, 4096, 0);

    if (bytes_received < 0) {
      std::cerr << "[SERVER] [ERROR]> Recieve error" << std::endl;
      hasError = true;
      return;
    }

    recieve_message = buff;
  }

  void Send_Socket(const std::string &send_message) {

    int send_ret =
        send(temp_client_fd, send_message.c_str(), send_message.size(), 0);

    std::cout << "[SERVER]> Sending message..." << std::endl;

    if (send_ret < 0) {
      std::cerr << "[SERVER] [ERROR]> Send error" << std::endl;
    }
  }

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

int main() {

  using namespace std;

  TCP_Server tcp_server;

  tcp_server.Init_Socket();
  if (tcp_server.GetHasError()) {
    return -1;
  }

  tcp_server.Bind_Socket();
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

  std::string recieve_message;
  tcp_server.Receive_Socket(recieve_message);
  std::cout << "[CLIENT]> \n" << recieve_message << std::endl;

  // Just try to make HTTP response
  std::string server_message =
      "HTTP/1.1 200 OK\nServer:JOJO\n\n<p>Hello world</p>\n";
  tcp_server.Send_Socket(server_message);

  return 0;
}
