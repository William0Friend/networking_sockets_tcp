#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
//socket_in structure, ipv4 version of the generic socket_addr
#include <netinet/in.h>

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
    int sock, length;
    struct sockaddr_in server, client;
//create socket connection, check for socket connection error, if good let that data flow
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        perror("socket: failed to connect, ain't no data a'flowin'!");
        exit(-1);
    }
//create socket sddress
//server_mine.sin_family = AF_INET;
//server_mine.sin_port = htons(8889);
//server_mine.sin_addr.s_addr = INADDR_ANY;


//create socket sddress
server.sin_family = AF_INET;
server.sin_port = htons(8889);
server.sin_addr.s_addr = INADDR_ANY;
//zero out sin_zero to pad the segement. 
// Padding is so the the semegemt has breathing room to cast freely to different types. 
//bzero(&server.sin_zero, 8);
bzero(&ser)
//we need to give a value to bind, so that it knows what size variable it will need to bind to.
//In our case we need the same size as our socket address, example '127.0.0.1:80' or 'sin_addr:sin_port'
length = sizeof(struct sockaddr_in);
//bind
//
//if true, bind socket connection, check for socket bind error, if false
//If binding fails print error
if((bind(sock, (struct sockaddr *) &server, length)) == -1 )
{
    perror("bind error, you've been ghosted");
    return -1;
}
}