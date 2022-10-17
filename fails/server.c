#include <stdio.h> //printf(), scanf()
#include <sys/socket.h> //sockaddr,
#include <sys/types.h> //
#include <stdlib.h> //malloc(), free()
#include <netinet/in.h>

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
/*
The <sys/socket.h> header shall define the size_t and ssize_t types  as
       described in <sys/types.h>.

       The following shall be declared as functions and may also be defined as
       macros. Function prototypes shall be provided.

           int     accept(int, struct sockaddr *restrict, socklen_t *restrict);
           int     bind(int, const struct sockaddr *, socklen_t);
           int     connect(int, const struct sockaddr *, socklen_t);
           int     getpeername(int, struct sockaddr *restrict, socklen_t *restrict);
           int     getsockname(int, struct sockaddr *restrict, socklen_t *restrict);
           int     getsockopt(int, int, int, void *restrict, socklen_t *restrict);
           int     listen(int, int);
           ssize_t recv(int, void *, size_t, int);
           ssize_t recvfrom(int, void *restrict, size_t, int,
                   struct sockaddr *restrict, socklen_t *restrict);
           ssize_t recvmsg(int, struct msghdr *, int);
           ssize_t send(int, const void *, size_t, int);
           ssize_t sendmsg(int, const struct msghdr *, int);
           ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *,
                   socklen_t);
           int     setsockopt(int, int, int, const void *, socklen_t);
           int     shutdown(int, int);
           int     sockatmark(int);
           int     socket(int, int, int);
           int     socketpair(int, int, int, int [2]);
*/
}

/*
//in sys/socket.h
struct sockaddr?{
  unsigned short sa_family; //address family, in this case we want AF_INET
  char sa_sata[14];  // Family specific addrees information, can fit ipv4 
}
*/
struct in_addr{
  unsigned long s_addr; //internet 32 bit address
};
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
  */

struct sockaddr_in{
  unsigned short sin_family; //address family, in this case we want AF_INET
  unsigned short sin_port; // assign the port
  struct in_addr sin_addr; // assign the ip address
  char sin_ero[8]; //errors
};

  //IPv4 address layout with .sin_port and .sin_addr
  
int main(int argc, char * argv[])
{
/*
  //print and take in commad line arguments main
    int i;
    for(i = 0; i < argc; i++){
        std::cout << "Argument "<< i << " = " << argv[i] << std::endl;
    }
*/
  /*
The server will be invoked (from the command line) thus:
server <port-number>
Where <port-number> is any numeric port number that the user provides at run time via
the command line. The server attempts to bind a TCP socket to this port and listen
for connections (if the server cannot bind to this port, the server will exit after printing an
informative error message for the user).
*/
  //handle with make file
  
  /*
3. When the server receives the text message, it will display the message to standard output
(including the IP address of the machine that sent the message), and send an acknowledgement
back to the client in the form of a text message: “Message received.”
*/
  /*The <sys/socket.h> header shall define the  sockaddr  structure,  which
       shall include at least the following members:
           sa_family_t  sa_family  Address family.
           char         sa_data[]  Socket address (variable-length data)
* Structure describing a generic socket address.  
struct sockaddr
  {
    __SOCKADDR_COMMON (sa_);    // Common data: address family and length.  
    char sa_data[14];       // Address data.  
  };
  */
 //generic socket
  //struct sockaddr  server_address;
  //IPv4 address layout with .sin_port and .sin_addr
 /*
  struct sockaddr_in serveraddress;
  serveraddress2.sin_family = AF_INET;
*/
/*  //PYTHON BOOK EXAPMLE CODE
from socket import *
serverPort = 12000
serverSocket = socket(AF_INET,SOCK_STREAM)
serverSocket.bind((’’,serverPort))
serverSocket.listen(1)
print(’The server is ready to receive’)
while True:
  connectionSocket, addr = serverSocket.accept()
  sentence = connectionSocket.recv(1024).decode()
  capitalizedSentence = sentence.upper()
  connectionSocket.send(capitalizedSentence.encode())
  connectionSocket.close()
*/

  //hold ip
int server_file_descriptor = 0, client_file_descriptor = 0;
  //hold message
struct sockaddr_in server_address, client_address;
  //unsign interger value
socklen_t address_size = 0;
int len = sizeof(client_address);


int number_recieved;
char segment_message[100001] ;

  //hold mesage up to 100k or 100000 kb

//socket(family, type, protocol) - posix system call
/* 
listen_file_desriptor is a file descriptor.
family: It is an integer in communication domain.
There are 2 possible domains:
1. Unix Domain: Where 2 process share the same file system. In that case family will be “AF_UNIX”
2. Internet Domain: They are 2 hosts on the Internet.In that case family will be “AF_INET”
type: It is the type of communication.
SOCK_STREAM: For TCP
SOCK_DGRAM: For UDP
protocol: It will specify the protocol.
IPPROTO_TCP, IPPROTO_UDP.
But it is always set to 0, so that OS will choose appropriate protocol.
This API will return -1 upon failure.
*/
  server_file_descriptor = socket(AF_INET,SOCK_STREAM, 0);
  //memset sets any memory olaction it's given to the value it is givin
  //this just zero's out
  //memset(&server_file_descriptor, '0',sizeof(server_file_descriptor)); 
  //memset(segment_message, '0',sizeof(segment_message)); 
//most x86_64 is little endian, but the network is big endian so we will have to check
    //check endian
if(check_if_little_endian())
{
  //set server_generic attributes
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);
  server_address.sin_addr.s_addr = INADDR_ANY;
} 
else
{
  //set server_generic attributes
  server_address.sin_family = AF_INET;
  server_address.sin_port = 8888;
  server_address.sin_addr.s_addr = INADDR_ANY;
}
/* //bind
sockid : It is the socket descriptor that we created earlier.
addrPort: it is the filled part of the structure sockaddr_in, casted to sockaddr.
size: It is the size of sockaddr_in.
status: It will return -1 if failure.
*/
bind(server_file_descriptor, (struct sockaddr *) &client_address, &len);
/* //listen
int status = listen(sockID, queueLimit);
sockID: It is the socked file descriptor that we created earlier.
queueLimit: It is the number of active participants that can wait for a connection.
status: -1 if error.
*/
if(listen(server_file_descriptor, 5) == 0);
/* //connect //open data flow
int status = connect(sockID, &serverAddr, addrLen);
sockID: It is the socket file descriptor.
serverAddr: Filled address of “sockaddr_in” structure, casted to “sockaddr”
addrLen: It is the length of the address.
status: -1 for failure to connect to server.
*/
//connect(listen_file_descriptor, &client_address, &len);
/* //accept
int sock = accept (sockID, &clientAddr, &addrLen);
sock: It is the new socket used for data transfer.
sockID: It is socket file descriptor.
clientAddr: It is the “sockaddr_in” variable, the address of the client. It will be filled upon return.
addrLen: It is the size of clientAddr.
*/
client_file_descriptor = accept (server_file_descriptor, (struct sockaddr *) & client_address, &len);
/* //send
int count = send(sockid, msg, msgLen, flags)
msg: It is the message to be transmitted.
msgLen: Length of the message
flags: Special options, usually 0
count: Number of bytes transmitted, -1 if error
*/
send()
/* //recieve
int count = recv(sockid, recvBuf, bufLen, flags)
recvBuf: It is the message to be received.
bufLen: Length of the message
flags: Special options, usually 0
count: Number of bytes received, -1 if error
*/

//struct sockaddr server_addr;
//server_addr.sa_family = AF_INET;
//server_addr.sa_data.s_addr = &argv[1];//(char) INADDR_ANY;
  /*
4. The client and server will then drop the TCP connection and the server will continue listening
for new connections.
*/
  /*
5. You must implement your assignment such that it meets the following requirements:
• Implemented in the C programming language1
• Compiles with the GCC or Clang C compiler
• Is POSIX compliant code, and uses the sys/socket (and perhaps the sys/types) libraries
*/
  /*

6. You must submit your complete source code, with a block-style header comment which includes
at the very least, your name (or names for undergrads that worked in pairs), our class number,
and the date.
*/
  /*
7. Make sure that your code is well documented and uses appropriate naming conventions for all
variables, procedures, methods, and classes (as appropriate). If your code is not well documented
and does not follow appropriate naming conventions, you will lose points.
*/
  /*
8. Test your code. You will submit a checklist with your code that indicates if your project compiles
or not, if it runs or not, and if it runs with errors. Be honest. If you are not honest on your
checklist, you will lose points.
*/
  /*
9. Your client/server must be able to handle a message of arbitrary size, from small to rather large
(somewhere around 100KB). When I test your program, I will redirect the contents of a text file
to the standard input of the client. The max file size that I will use will be around 100K.
10. Make sure you read from standard input and display the message result to standard output.
There is no need for any file I/O in this assignment.
11. If your server can not bind to the port given on the command line, it should display an
appropriate message to standard error and then gracefully cleanup/exit
12. If the client is unable to bind/connect with the server, it should also display an appropriate
message to standard error and then gracefully cleanup/exit.
13. Note well: I am keenly aware that many examples of simple TCP client/server messaging code
exist on the Internet. Don’t sell yourself short. Write the code yourself and learn more about
TCP and sockets. Your future boss will like you for it. Plus, I use a rather sophisticated
plagiarism detection tool (not the stock D2L plagiarism essay checker). I would hate to have to
send business to the student conduct board at this stage of the game.
14. In the example below, the server is running on a machine with IP address 192.168.1.2 and the
client is on a machine with IP address 192.168.1.4. You will provide your own message (test withmessages of various length), IP address, and port number. You will not type the five characters
“<EOF>” but type the single <EOF> character.2
Example input/output for the client is shown below. Output is in blue and input is in red (the
format of your input/output must comply with the example below):
client 192.168.1.2 62637
Some interesting and exciting message.<EOF>
Message received.
Example input/output for the server is shown below. Output is in blue and input is in red (the
format of your input/output must comply with the example below):
server 62637
Message from 192.168.1.4:
Some interesting and exciting message.





    return 0;
}
