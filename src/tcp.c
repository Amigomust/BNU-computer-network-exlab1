// tcp.c

#include "tcp.h"

void sig_handler(int sig) {
  if(sig == SIGINT) {
    closesocket(sk);
    WSACleanup();
    printf("[Info] Close socket listening.\r\n");
    exit(0);
  }
}

int tcp() {
  WSADATA wsaData;
  SOCKADDR_IN addrServer;
  SOCKET socketClient;
  SOCKADDR_IN addrClient;

  int addrlen = sizeof(SOCKADDR);
  char recvBuf[RECV_BUFFER_LENGTH];
  char *sendBuf;

  WSAStartup(MAKEWORD(2, 2), &wsaData);
  sk = socket(AF_INET, SOCK_STREAM, 0);
  addrServer.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
  addrServer.sin_family = AF_INET; // IPv4
  addrServer.sin_port = htons(80);
  bind(sk, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
  
  if(listen(sk, BACKLOG_LENGTH) == 0) {
    printf("[Info] Start to listen 0.0.0.0:80\r\n");
    signal(SIGINT, sig_handler);
    while(1) {
      socketClient = accept(sk, (SOCKADDR*)&addrClient, &addrlen);
      if(socketClient != -1) {
        recv(socketClient, recvBuf, RECV_BUFFER_LENGTH, 0);
        sendBuf = http_process(recvBuf);
        if(sendBuf) {
          send(socketClient, sendBuf, strlen(sendBuf), 0);
        }
        closesocket(socketClient);
      }
    }
  } else {
    printf("[Error] Cannot listen.\r\n");
  }
  return 0;
}
