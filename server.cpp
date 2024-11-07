#include "src/tcp_server.h"

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
