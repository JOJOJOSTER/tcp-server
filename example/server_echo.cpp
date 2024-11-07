#include "../src/tcp_server.h"

bool PrintServerMessage(ERROR_HANDLER error_code) {
  if (error_code.isSuccesful()) {
    std::cout << "[SERVER]> " << error_code.Get_Message() << std::endl;
  } else {
    std::cout << "[SERVER] [ERROR]> " << error_code.Get_Message() << std::endl;
  }

  return error_code.isSuccesful();
}

int main() {
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

  while (true) {

    std::string recieve_message;

    ERROR_HANDLER ret_error = tcp_server.Receive_Socket(recieve_message);

    if (ret_error.isSuccesful()) {
      std::cout << "[SERVER] " << ret_error.Get_Message();
    } else {
      std::cout << "[SERVER] [ERROR]> " << ret_error.Get_Message() << std::endl;
      break;
    }

    std::cout << "[CLIENT] [Message]> " << recieve_message << std::endl;

    tcp_server.Send_Socket(recieve_message);
  }
}
