#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
//socket_in structure, ipv4 version of the generic socket_addr
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#define SIZE100K 100000
#define MY_SOCK_PATH "/home/break/"
//could use 
/*
#include <netinet/tcp.h>
#include <sys/un.h>
#include <netinet/udp.h>
*/
/*
//in sys/socket.h
struct sockaddr?{
  unsigned short sa_family; //address family, in this case we want AF_INET
  char sa_sata[14];  // Family specific addrees information, can fit ipv4 
}
struct in_addr{
  unsigned long s_addr; //internet 32 bit address
};
*/
  /*
  struct sockaddr_in
  {
    __SOCKADDR_COMMON (sin_);
    in_port_t sin_port;         // Port number.  
    struct in_addr sin_addr;        // Internet address.  

    // Pad to size of `struct sockaddr'.  
    unsigned char sin_zero[sizeof (struct sockaddr) -
               __SOCKADDR_COMMON_SIZE -
               sizeof (in_port_t) -
               sizeof (struct in_addr)];
  };

struct sockaddr_in_mine{
  unsigned short sin_family; //address family, in this case we want AF_INET
  unsigned short sin_port; // assign the port
  struct in_addr sin_addr; // assign the ip address
  char sin_zero[8]; //errors
};

  */

int main(){
    //int sockfd = 0, cli = 0, i = 0, numrv = 0;
    int sockfd = 0, length = 0, clifd = 0, sent = 0, numrv = 0;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char message[] = "hello from socket programming hel! moo wah ha ha ha! We're connected buddy";
    char msg[SIZE100K];
//create socket connection, check for socket connection error, if good let that data flow
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        perror("socket: failed to connect, ain't no data a'flowin'!");
        exit(EXIT_FAILURE);
     }
       printf("socket retrieve success\n");
//create socket sddress
//server_mine.sin_family = AF_INET;
//server_mine.sin_port = htons(8889);
//server_mine.sin_addr.s_addr = INADDR_ANY;

//zero out sin_zero to pad the segement. 
// Padding is so the the semegemt has breathing room to cast freely to different types. 
//bzero(&server.sin_zero, 8);
//This zeros out the padding
memset(&server_addr, '0', sizeof(server_addr));

//create socket sddress
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(10001);
server_addr.sin_addr.s_addr = INADDR_ANY;

strncpy(server_addr.sin_path, MY_SOCK_PATH, sizeof(server_addr.sin_path))
//we need to give a value to bind, so that it knows what size variable it will need to bind to.
//In our case we need the same size as our socket address, example '127.0.0.1:80' or 'sin_addr:sin_port'
length = sizeof(struct sockaddr_in);
//bind
//if true, bind socket connection, check for socket bind error, if false
//If binding fails print error
if((bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) == -1 )
{
 //   perror("bind error, you've been ghosted");
   // exit(EXIT_FAILURE);
   handle_error("bind server");
}

if ((listen(sockfd, 10))== -1)
{
    perror("listen is down, in space no one can hear you scream");
    exit(EXIT_FAILURE);
}

while(1)
{
    /*
Await a connection on socket FD.
When a connection arrives, open a new socket to communicate with it,
set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
peer and *ADDR_LEN to the address's actual length, and return the
new socket's descriptor, or -1 for errors.
This function is a cancellation point and therefore not marked with
__THROW.
    */
    if((clifd = accept(sockfd,(struct sockaddr *)&client,&length))==-1)
    {
        perror("accept, i refused to accept you for who you are, rejected");
        return EXIT_FAILURE;
    }
    //we need to seed the welcome message
    sent = send(clifd, message, strlen(message), 0);

    printf("Sent %d bytes to client : %s \n", sent, inet_ntoa(client.sin_addr));

    //
for(;;){
    bzero(msg, sizeof(msg));
    //read the message from the client and copy it to the buffer
    read(sockfd, msg, sizeof(msg));
}
    printf("msg: %s", msg);
}
//close any open file descriptor
    close(clifd);
    close(sockfd);
    return 0;
}
