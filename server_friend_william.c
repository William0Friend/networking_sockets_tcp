/*---------------------------------------------*
 *                                             *
 * Author: William Friend                      *
 * Date: 10/18/2022                            *
 * Server.c                                    *
 * This is the driver for the server.c sockets *
 *  programming exercise.                      *
 *                                             *
 * This follows the standard sequence          *
 * socket                                      *
 * bind                                        *
 * listen                                      *
 * accept                                      *
 *                                             *
 * Server runs an infinate loop reading from   *
 * clients and recieving messages of up to     *
 * 100kb                                       *
 *                                             *
 * Server then sends a client a response       *
 *                                             *
 * Server's session runs inifinately
 * 
 *
 * IMPORTANT:
 * Problem with ftell() on server side
 * ftell is not giving me the correct size
 * I get -1 when on client this gives correct size
 * If I could get the size of the clients message somehow
 * this would work perfectly
 * I wanted the sleep() to make the server wait long enough to 
 * get the connfd/sockfd(client side/server side) stream full
 * before I tried to get the size, but it is only returning -1
 * functionality not complete,
 * at the moment the client needs a ctrl^c signal
 * to terminate 
 * so basically the length it is measuring isn't long enough
 * 
 * I can make the message print at any size by making the
 * first fread too large of a size
 * but then I need to ctrl-c on client for the server to print the messages
 * 
 * 
 * Server otherwise works fine, however the <EOF>
 * is not terminating the fread() sequence
 * 
 * 
 * 
 * 
 * Otherwise,
 * 
 * 
 * This does everything that is expected of it
 * This works on two seperate computers
 * It prints the clients ip above message
 * It uses sys/types.h and sys/socket.h
 * It is posix compliant since,
 * it basicaly only uses gcc, c, and glibc
 * It reads, stores, and prints up to 100KB
 * Then it sends recieve message
 * 
 *  
 * 
 * 
 * 
 * 
 *---------------------------------------------**/


// Libraries needed
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sha2.h>
#include <errno.h>
#include <error.h>
#define SA struct sockaddr
#define LISTENQ 5
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 110 // breathing room...
#define MSGSIZE 30
int e = 0; // for error check



// Driver code
int main(int argc, char **argv)
{
    int sockfd, listenfd, connfd;// file descriptors
    socklen_t client;//client ip address
    char buf[100];//hold client ip address as string
    struct in_addr addr;//copy of ip address
    int bits;
    struct sockaddr_in servaddr, cliaddr;//create stuct
    unsigned char *buffer = NULL;
    int stdoutfd = fileno(stdin);
//+++++++++++++++++++++++++++++++++++++++++++++    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);// socket file descriptor
//+++++++++++++++++++++++++++++++++++++++++++++    
    bzero(&servaddr, sizeof(servaddr));//zero out our memory
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//assign ip, with correct endianess
    servaddr.sin_port = htons(atoi(argv[1])); //assign port, with correct endianess
//+++++++++++++++++++++++++++++++++++++++++++++    
    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));// bind to the socket
//+++++++++++++++++++++++++++++++++++++++++++++    
    listen(listenfd, LISTENQ);// start listening for new clients
//+++++++++++++++++++++++++++++++++++++++++++++    
    client = sizeof(cliaddr); //size of clients ip address
    while (1)// infinite server loop, ctrl-c to terminate
    {
//+++++++++++++++++++++++++++++++++++++++++++++    
        connfd = accept(listenfd, (SA *)&cliaddr, &client);// create file pointer to easily write large amounts over socket
//+++++++++++++++++++++++++++++++++++++++++++++    
        FILE *connFp = fdopen(dup(connfd), "r+");   // convert file descriptor to streamm
                                                    // for reading and writing to
        //FILE *stdoutFp = stdout;                    // we want our output file to be stdout
        FILE *stdoutFp = fdopen(STDOUT_FILENO,"w");  //so we open a stream to stdout
                                                    //for reading and writing
        addr.s_addr = cliaddr.sin_addr.s_addr;//get clients address from struct
        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) == NULL)//
        {
            printf("\nMessage from 0.0.0.0\n ");
        }
        printf("\nMessage from  %s:\n ", buf);
//+++++++++++++++++++++++++++++++++++++++++++++ 
//Measure the file and allocate memory
        //fseek(connFp,0,SEEK_SET);
        //sleep(5);
        fseek(connFp, 0, SEEK_END);
        unsigned long fileSize = ftell(connFp);
        rewind(connFp);
        char *buffer = (char*)malloc(fileSize+1);
        buffer[fileSize] = '\0';
//Save the contents of the file to a variable
        //printf("size of buffer: %d ", fileSize);

//I call this the "goldylocks problem"
        //when I hard code it like this it works perfect;
//        fread(buffer, sizeof(char), 23, connFp); //exact size
//        fread(buffer, sizeof(char), COPY_BUFFER_MAXSIZE, connFp); //too big
        fread(buffer, sizeof(char), fileSize, connFp); //to small
       //+++++++******************
        //When I know the size o the clients message size, the program is
        //perfect, I just can't figure this out
        //otherwise I keep overshooting or under shooting the size of the clients message
        //when i over shoot, client need control c not eof
        //and when I undershoot server is not copying message, so it not being printed out.
       //*********
       //since it doesn't fail if I make it to small Ill leave it like that but 
       //you can uncomment and swicth out sizes if you want, if you put in the message size exactly 
       //though it works perfect
       //********
       //++++++ *****************

        //rewind(stdoutFp);
//        fwrite(buffer,sizeof(char), 23, stdoutFp);//exact size
//        fwrite(buffer,sizeof(char), COPY_BUFFER_MAXSIZE, stdoutFp);
        fwrite(buffer,sizeof(char), fileSize, stdoutFp);

        //write(STDOUT_FILENO, buffer,sizeof(buffer));
//+++++++++++++++++++++++++++++++++++++++++++++    
        fflush(stdout);//flush output?
        //for (int i = 0; i < fileSize; i++)// print 100kb message 
        //{
        //    printf("%c", buffer[i]);
        //}
        fflush(connFp);
        fflush(stdoutFp);        
        char buff2[MSGSIZE] = "Message Recieved\n\0 ";//create user message
        int n;
//+++++++++++++++++++++++++++++++++++++++++++++    
        write(connfd, buff2, (size_t)18);//write user message
//+++++++++++++++++++++++++++++++++++++++++++++    
        printf("\n\nthis only shows up when server can\n\t get size of message ahead of time\n\n");
        //printf("Sent Message recieved to client\n\0");
        printf("\n\n\n\n\nAccepting new clients..\n\0");
        fflush(connFp);
        fflush(stdout);
    } // END while(true)
    // close up shop
    free(buffer);
    close(connfd);
    printf("\n\n\nServer work is never done...\n\n\n\0");
    return 0;
} 