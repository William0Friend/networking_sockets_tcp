#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
//socket_in structure, ipv4 version of the generic socket_addr
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define SIZE100K 100000
#define SIZE1024 1024


int check_if_little_endian()
{
    int a = 0x12345678;
    unsigned char *c = (unsigned char*)(&a);
    if (*c == 0x78) {
       //printf("little-endian\n");
       return 1;
    } else {
       //printf("big-endian\n");
       return 0;
    }
}


int main(int argc, char * argv)
{

    int sockfd = 0, i = 0;
    char msg255[255]; //one less then the server message
    char msg100k[SIZE100K];

    //memset(msg, '0', sizeof(recvmsg));

//create socket
struct sockaddr_in sock_addr;
//Zero out a selected chunk of memory, or our 
// struct sock_addr could have old values, 
// so we zero out
//memset(&sock_addr, '0', sizeof(sock_addr));
memset(msg100k, '0', sizeof(msg100k));

if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
    perror("socket client");
    exit(EXIT_FAILURE);
}

//create socket sddress
sock_addr.sin_family = AF_INET;
sock_addr.sin_port = htons(10001);
sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

//connect to server
if(connect(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr))== -1)
{
    perror("connect client");
    exit(EXIT_FAILURE);
}

/*
//recieve servers welcome
recv(sockfd, msg255, strlen(msg255), 0);
printf("%s\n", inet_ntoa(msg255));
*/
//we need to print the welcome message out
//char buff[SIZE];
int j = 0;
for(;;)
{
    //zero out msg
    //bzero(msg100k, sizeof(msg100k));
    //add stdin to msg, one character at a time.
    //until eof
    while ((msg100k[j++] = getchar() != EOF ));
    // write sendmsg to sockfd
    if(write(sockfd, sendmsg, sizeof(sendmsg)) == -1)
    {
        perror("write client");
        exit(EXIT_FAILURE);
    }
/*

      strcpy(sendBuff, "Message from server");
      write(connfd, sendBuff, strlen(sendBuff));
 
      close(connfd);    
      sleep(1);
*/
    return 0;
}
/*
//take initial input from the 
 //print and take in commad line arguments main
    int i;
    for(i = 0; i < argc; i++){
        std::cout << "Argument "<< i << " = " << argv[i] << std::endl;
    }
*/
}