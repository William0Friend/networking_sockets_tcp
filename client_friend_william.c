/*---------------------------------------------**
 *                                             **
 * Author: William Friend                      **
 * Date: 10/18/2022                             *
 * Client.c                                     *
 * This is the driver for the client.c sockets  *
 * programming exercise.                        *
 *                                              *
 * This follows the standard sequence           *
 * socket                                       *
 * connect                                      *
 *                                              *
 * Client runs it's sequence once reading       *
 * from stdin and writing to the connection     *
 *                                              *
 * The max input size is 100kb                  *
 *                                              *
 * Client then recieves a response from the     *
 * server and prints that out                   *
 *                                              *
 * Client then ends it's session                *
 *                                              *
 *
 * 
 * IMPORTANT:
 * functionality not complete,
 * at the moment the client needs a ctrl^c signal
 * to terminate 
 *
 *  Server otherwise works fine, however the <EOF>
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
 *---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sha2.h>
#include <errno.h>
#define SA struct sockaddr
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 100
#define MSGSIZE 19

int main(int argc, char **argv)
{   
    int e;
    int sockfd;// file descriptors
    int len;
    struct sockaddr_in servaddr, cliaddr;
    //create stuct
//+++++++++++++++++++++++++++++++++++++++++++++    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);// socket file descriptor
//+++++++++++++++++++++++++++++++++++++++++++++    
    bzero(&servaddr, sizeof(servaddr));//zero out our memory
    int num = atoi(argv[2]);//take in port number
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//assign ip, with correct endianess
    servaddr.sin_port = htons(atoi(argv[2])); //assign port, with correct endianess
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);//convert port number to ipv4
//+++++++++++++++++++++++++++++++++++++++++++++    
    connect(sockfd, (SA *)&servaddr, sizeof(servaddr));//conect to the server
//+++++++++++++++++++++++++++++++++++++++++++++    
    //unsigned char *buffer = NULL;//to hold the 100kb on the heap, staack wouldn;t be big enough
    //buffer = malloc(COPY_BUFFER_MAXSIZE);//allocate the 100kb
    //if (!buffer)//make sure it's allocated
    //    exit(-1);
    FILE *stdinFp = fdopen(STDIN_FILENO, "r");// FILE ptr for read from stdin
    FILE *sockFp = fdopen(dup(sockfd), "w+");//turn file descriptor into stream, easier with malloc
    int stdinfd = fileno(stdin);//file descriptor for stdin, could have used STDIN_FILENO
    //fseek(stdin, 1, SEEK_END);
    //size_t stdinLength = ftell(stdin);
    //fseek(stdin, 1, SEEK_SET);
    rewind(stdinFp);
//+++++++++++++++++++++++++++++++++++++++++++++    `
    fseek(stdinFp, 0, SEEK_END);
    unsigned long fileSize = ftell(stdinFp);
    rewind(stdinFp);
    char *buffer = (char*)malloc(fileSize+1);
    buffer[fileSize-1] = '\0';
    printf("sent buffer, used %db out of possible 100kb\n",fileSize);
    fread(buffer, sizeof(char), (size_t)fileSize, stdinFp);//read 100kb to stream/socket
//+++++++++++++++++++++++++++++++++++++    
    rewind(stdinFp);
    fwrite(buffer, 1, (size_t)fileSize, sockFp);//write 100kb to socket
    char end[2] = "\n\0";
    fwrite(end, sizeof(char), (size_t)end, sockFp);;//append null terminator?
//+++++++++++++++++++++++++++++++++++++++++++++    
    fflush(stdin);
    fflush(sockFp);
    //printf("sent 100kb mesage\n");
    char buffer2[MSGSIZE];
    bzero(buffer2, sizeof(buffer2));
    FILE *ackFp = fdopen(dup(sockfd), "rb");
    FILE *stdoutFp = stdout;
//+++++++++++++++++++++++++++++++++++++++++++++    
    read(sockfd, buffer2, (size_t)sizeof(buffer2));//read "Message Recieved from server"
//+++++++++++++++++++++++++++++++++++++++++++++    
    //printf("read ack \n");//acknoledge we got response
    printf("%s\n", buffer2);//print the message
    fflush(stdoutFp);//flush stdout
    fflush(stdout);
    // close shop
    fflush(stdoutFp);
    free(buffer);
    close(sockfd);
    return 0;
}
/*        if (read(sockfd, buffer2, (size_t)sizeof(buffer2)) == -1) {
        switch (errno)
        {
        case 1:
            perror("case 1:\n");
            break;
        case 2:
            perror("case 2:\n");
            break;
        case 3:
            perror("case 3:\n");
            break;
        case 4:
            perror("case 4:\n");
            break;
        case 5:
            perror("case 5:\n");
            break;
        case 6:
            perror("case 6:\n");
            break;
        case 7:
            perror("case 7:\n");
            break;
        case 8:
            perror("case 8:\n");
            break;
        case 9:
            perror("case 9:\n");
            break;
        default:
            printf("default\n");
            break;
        }
    }
*/