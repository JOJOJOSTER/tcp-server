#include "../src/tcp_server.h"

int main() {
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

  while (true) {

    std::string recieve_message;

    int ret_recieve = tcp_server.Receive_Socket(recieve_message);

    if (ret_recieve <= 0) {
      std::cout << "[CLIENT]> Disconnected" << std::endl;
      break;
    }

    std::cout << "[CLIENT] [Message]> " << recieve_message << std::endl;

    tcp_server.Send_Socket(recieve_message);
  }
}
