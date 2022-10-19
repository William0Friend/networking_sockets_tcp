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
 * functionality not complete,
 * at the moment the client needs a ctrl^c signal
 * to terminate 
 * 
 * Server otherwise works fine, however the <EOF>
 * is not terminating the fread() sequence
 * 
 * Otherwise,
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
    struct sockaddr_in servaddr, cliaddr;
    unsigned char *buffer = NULL;
    int stdoutfd = fileno(stdin);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);// socket file descriptor
    bzero(&servaddr, sizeof(servaddr));//zero out our memory
    servaddr.sin_family = AF_INET; //create stuct
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//assign ip, with correct endianess
    servaddr.sin_port = htons(atoi(argv[1])); //assign port, with correct endianess
    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));// bind to the socket
    listen(listenfd, LISTENQ);// start listening for new clients
    client = sizeof(cliaddr); //size of clients ip address
    while (1)// infinite server loop, ctrl-c to terminate

    {
        connfd = accept(listenfd, (SA *)&cliaddr, &client);// create file pointer to easily write large amounts over socket
                                                           // works easier with malloc way
        FILE *connFp = fdopen(dup(connfd), "r+");   // convert file descriptor to streamm
                                                    // for reading and writing to
        //FILE *stdoutFp = stdout;                    // we want our output file to be stdout
        FILE *stdoutFp = fdopen(STDOUT_FILENO,"w+");  //so we open a stream to stdout
                                                    //for reading and writing
        addr.s_addr = cliaddr.sin_addr.s_addr;//get clients address from struct
        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) == NULL)//
        {
            printf("\nMessage from 0.0.0.0\n ");
        }
        printf("\nMessage from  %s:\n ", buf);
        buffer = malloc(COPY_BUFFER_MAXSIZE);//  assign memory for the message.
        memset(buffer, 0, (size_t)sizeof(buffer)); // zero out memory
        if (!buffer)// exit if buffer has error
            exit(-1);
        fseek(connFp, 1, SEEK_END);//get client message length
        int connfdLength = ftell(connFp);//assign length to connfdLength
        rewind(connFp);//send pointer back to the front of the stream
        //  fread(buffer, 1, (size_t)connfdLength, connFp) != NULL;
        // read 100kb message - 1
        // if (!feof(connFp))
        // fread(buffer, 1, (size_t)connfdLength, connFp);
        // read in - 100kb message
        if (!feof(connFp))//check for <EOF> in stream, if not there, read
            fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, connFp);//upto 100Kb
        // read(dup(connfd), buffer, (size_t)(buffer));
        //printf("a1\n");
        // fflush(connFp);
        // read 100kb message - 2
        // if(!feof(connFp))
        //     fgets(buffer, COPY_BUFFER_MAXSIZE, connFp);

        // write out - 100kb message
        //        if(!feof(connFp))
        // write(STDOUT_FILENO, buffer, (size_t)(out));
        // fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, stdout); // read(connfd, buffer, (size_t)COPY_BUFFER_MAXSIZE);
        //printf("a2\n");
        fflush(stdout);//flush output?
        // if(!feof(connFp)){
        //     fputs(buffer,stdout);
        // }
        // print 100kb message - 1
        // if((write(stdinfd, buffer, (size_t)sizeof(buffer)) > 0));
        int out = strlen(buffer);//get string length again to compare eventually
        for (int i = 0; i < out; i++)// print 100kb message
        {
            printf("%c", buffer[i]);
        }
        fflush(connFp);
        fflush(stdout);        
        //        for (int i = 0; i < 1; i++)
        //        {
        //printf("2\n");
        // write message to client
        //             x = fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
        //   printf("3\n");
        // fflush(outFp);
        //        }
        //    ++i;
        //   break;
        //printf("\nEXIT 1\n");
        //        puts("\n");
        // we are on to the next
        //        sha256Checksum(buffer);
        // send messages
        // char buffer2[MSGSIZE];
        // bzero(buffer2, sizeof(buffer2));

        //       printf("EXIT 2\n");
        //        puts("\n");
        // b2) ack to 0b - 100k ie response message
        char buff2[MSGSIZE] = "Message Recieved\n\0 ";//create user message
        int n;
        // for (int i = 0; i < 18; i++)
        //{
        //  n = read(connfd, temp, sizeof(temp));
        n = write(connfd, buff2, (size_t)18);//write user message
        //}
        printf("Sent Message recieved to client\n\0");
        // puts("\n");
        // b3) send sha
        //        puts("\n");
        //        puts("\n");
        printf("Accepting new clients..\n\0");
        
        fflush(connFp);
        fflush(stdout);

    } // END while(true)
    // close up shop
    free(buffer);
    close(connfd);
    printf("\n\n\nServer work is never done...\n\n\n\0");
    return 0;
} 
        // alt read write 100k message
        // int x = 0;
        //        for (int i = 0; i < 1; i++)
        /* while(1) {
              c = fgetc(fp) != NULL;
              if( feof(fp) ) {
                 break ;
              }*/
        //printf("1\n");