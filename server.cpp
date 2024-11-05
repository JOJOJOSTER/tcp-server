#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  using namespace std;

  int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listeningSocket == -1) {
    cerr << "Can't create a socket" << endl;
    return -1;
  }

  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(54001);
  inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

  if (bind(listeningSocket, (sockaddr *)&hint, sizeof(hint)) == -1) {
    cerr << "Can't bind to IP/Port" << endl;
    return -2;
  }

  // SOMOXCONN -
  if (listen(listeningSocket, SOMAXCONN) == -1) {
    cerr << "Can't listen" << endl;
    return -3;
  }

  sockaddr_in clientAddress;
  socklen_t sizeClient = sizeof(clientAddress);

  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];

  int clientSocket =
      accept(listeningSocket, (sockaddr *)&clientAddress, &sizeClient);

  if (clientSocket == -1) {
    cerr << "Error connection to client" << endl;
    return -4;
  }

  close(listeningSocket);

  // clean mem from garbage
  memset(host, 0, NI_MAXHOST);
  memset(serv, 0, NI_MAXSERV);

  int result = getnameinfo((sockaddr *)&clientAddress, sizeof(clientAddress),
                           host, NI_MAXHOST, serv, NI_MAXSERV, 0);

  if (result) {
    cout << host << " connected on " << serv << endl;
  } else {
    inet_ntop(AF_INET, &clientAddress.sin_addr, host, NI_MAXHOST);
    cout << host << " connected on " << ntohs(clientAddress.sin_port) << endl;
  }

  char buff[4096];

  while (true) {

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
}
