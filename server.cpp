#include "src/tcp_server.h"

bool PrintServerMessage(ERROR_HANDLER error_code) {
  if (error_code.isSuccesful()) {
    std::cout << "[SERVER]> " << error_code.Get_Message() << std::endl;
  } else {
    std::cout << "[SERVER] [ERROR]> " << error_code.Get_Message() << std::endl;
  }

  return error_code.isSuccesful();
}

int main() {

  using namespace std;

  TCP_Server tcp_server;

  ERROR_HANDLER error_code = tcp_server.Start(54001, 1);

  if (!PrintServerMessage(error_code)) {
    return -1;
  }

  error_code = tcp_server.Accept_Socket();

  if (!PrintServerMessage(error_code)) {
    return -2;
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
