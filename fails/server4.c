/****************** SERVER CODE ****************/
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[]){
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
if((welcomeSocket = socket(PF_INET, SOCK_STREAM, 0)) != 0)
{
  perror("socket server");
  exit(1);
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
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  /*---- Bind the address struct to the socket ----*/
  if(bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0)
  {
      printf("bind server");
      //exit(0);
  }
printf("bind good");

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof(serverStorage);
  if((newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size)) != 0)
  { 
    printf("accept server");
  }
printf("accept good");
/*
  //---- Send message to the socket of the incoming connection ----
  strcpy(buffer,"Hello World\n");
  send(newSocket,buffer,13,0);
*/
  /*---- Read the message from the client into the buffer ----*/
  if(recv(welcomeSocket, buffer, 1024, 0) == -1)
  {
    perror("recv server");
  }
printf("recv good");
  //print the read message
  printf("%s", buffer);

  //read(welcomeSocket, )
  /*---- Print the received message ----*/
/*
printf("Data received: %s",buffer);   

      puts("The client sent the values: ");

       if(argc>=2){
      int i;
      for(i=0; i<argc; i++) {
      int* valuerec=(int *)buffer;
      int val2=*valuerec;
      printf("%d\n", val2); 
      puts(" ");
*/

  return 0;
}
