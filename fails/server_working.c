#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sha2.h>
#define SA struct sockaddr
#define LISTENQ 5
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 110
#define MSGSIZE 20
void sha256Checksum(unsigned char *buf);
// unsigned char * sha256GetChecksum(unsigned char *, unsigned char *);
void sha256GetChecksum(unsigned char *, unsigned char *);
int main(int argc, char **argv)
{
    // start server process
    // startserver();
    int fd, sockfd, listenfd, connfd;
    pid_t childpid;
    socklen_t client;
    char buf[100];
    struct in_addr addr;
    int bits;
    struct sockaddr_in servaddr, cliaddr;
    unsigned char *buffer = NULL;
    // a1) create socket file descriptor
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //    printf("socketing...\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    // a2) bind to port
    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    //    printf("binded...\n");
    // a3) listen for however many clients you want
    listen(listenfd, LISTENQ);
    //    printf("listening...\n");
    client = sizeof(cliaddr);
    // infinite server loop, ctrl-c to terminate
    for (;;)
    {
        connfd = accept(listenfd, (SA *)&cliaddr, &client);
        // printf("accepted client...\n");
        addr.s_addr = cliaddr.sin_addr.s_addr;
        // const char *inet_ntop(int af, const void *restrict src, char *restrict dst, socklen_t size);
        //           printf("inet_net_pton() returned: %d\n", bits);
        // get client's ip for output, default to 0.0.0.0
        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) == NULL)
        {
            printf("\nMessage from 0.0.0.0\n ");
        }
        printf("\nMessage from  %s:\n ", buf);
        // server now has connection
        // print Message up to 100-110kb
        //  assign memory for the message.
        buffer = malloc(COPY_BUFFER_MAXSIZE);
        // exit if buffer has error
        if (!buffer)
            exit(-1);
        // FILE *inFp = stdin;
        // create file pointer to easily write large amounts over socket
        // works easier with malloc way
        FILE *inFp = fdopen(dup(connfd), "rb"); // set file position indicator for stream pointed to by stream
        // set pointer to begining of file for sure
        FILE *outFp = stdout; // we want our output file to be stdout
                              // uint64_t outFileSizeCounter = fileSize;
        // fflush any symbols that may mess up our streams
        fflush(inFp);
        fflush(outFp);
        // read write 100k message
        int x = 0;
        for (int i = 0; i<1  ; i++)
        {
            printf("1\n");
            //x = fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, inFp);
            x = read(connfd,buffer,sizeof(buffer));
            ++i;

            //fflush(inFp);
            printf("1.5\n");
        }

        x = 0;
        for (int i = 0; x == 0; i++)
        {
            printf("2\n");
            x = fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
            printf("3\n");
            //fflush(outFp);
        }
        //    ++i;
         //   break;
        
        printf("EXIT 1\n");
        puts("\n");
        // we are on to the next
        //        sha256Checksum(buffer);
        fflush(outFp);
        // send messages
        //char buffer2[MSGSIZE];
        //bzero(buffer2, sizeof(buffer2));

        printf("EXIT 2\n");
        puts("\n");
        // b2) ack to 0b - 100k ie response message
        char buff2[MSGSIZE] = "Message Recieved\n ";
        int n;
        for (int i = 0; i < MSGSIZE/*buff2[i] != EOF*/; i++)
        {
            // n = read(connfd, temp, sizeof(temp));
            n = write(connfd, buff2, sizeof(buff2));
        }

        printf("EXIT 3\n");
        puts("\n");
        // b3) send sha
        printf("EXIT 4\n");
        puts("\n");
        printf("EXIT 5\n");
        puts("\n");
    } // end for(;;)

    // close up shop
    free(buffer);
    close(connfd);
    sleep(1);
    // printf("accepting new clients now outside while 1");
    printf("\n\n\nServer work is never done..:)\n\n\n");
    return 0;
}