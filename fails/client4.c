/****************** CLIENT CODE ****************/
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(){
  int clientSocket, connectSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  if(clientSocket = socket(PF_INET, SOCK_STREAM, 0)== -1)
  {
    //handle_error("socket client");
    perror("socket client");
    exit(-1);
  }
printf("socket good");
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7893);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) == -1)
  {
    //handle_error("connect client");
    perror("connect client");
    exit(-1);
  }
printf("connect good");

  //---- Read the message from the server into the buffer ----
  //recv(clientSocket, buffer, 1024, 0);
/*
  //---- Print the received message ----
  printf("Data received: %s",buffer);   
*/
  
    //add stdin to msg, one character at a time.
    //until eof
    //int j = 0;
    //while ((buffer[j++] = getchar() != '\0'));
  //---- Send message to the socket of the incoming connection ----
  //strncpy(buffer,"Hello World\n");
  
  //create message for server
  strcpy(buffer,"Hellp Server, from client");
  //send message to server
  if(send(clientSocket,buffer,sizeof(buffer),0) == -1)
  {
    perror("send client");
    exit(-1);
  }
  //print acknoldegement
  printf("sent good\n");

  return 0;
}
