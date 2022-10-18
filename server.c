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
#define MSGSIZE 20
int e = 0; // for error check
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
    int stdoutfd = fileno(stdin);
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
    while (1)
    {

        connfd = accept(listenfd, (SA *)&cliaddr, &client);
        // create file pointer to easily write large amounts over socket
        // works easier with malloc way
        FILE *connFp = fdopen(dup(connfd), "r+"); // set file position indicator for stream pointed to by stream
        // set pointer to begining of file for sure
        FILE *stdoutFp = stdout; // we want our output file to be stdout
                                 // uint64_t outFileSizeCounter = fileSize;
        int stdinfd = fileno(stdin);
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
        memset(buffer, 0, (size_t)sizeof(buffer));
        // exit if buffer has error
        if (!buffer)
            exit(-1);
        // FILE *inFp = stdin;
        // fflush any symbols that may mess up our streams
        // read write 100k message
        // int x = 0;
        int connfdLength = 0;
        //        for (int i = 0; i < 1; i++)
        /* while(1) {
              c = fgetc(fp) != NULL;
              if( feof(fp) ) {
                 break ;
              }*/
        printf("1\n");
        // fseek(connFp, 1, SEEK_END);
        // connfdLength = ftell(connFp);
        // fseek(connFp, 1, SEEK_SET);
        // rewind(connFp);
        //  fread(buffer, 1, (size_t)connfdLength, connFp) != NULL;
        // read 100kb message - 1
        // if (!feof(connFp))
        // fread(buffer, 1, (size_t)connfdLength, connFp);
        
        //read in - 100kb message
//        if (!feof(connFp))
//        fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, connFp);
        read(dup(connfd), buffer, (size_t)(buffer-1));
        printf("a1\n");
        // fflush(connFp);
        // read 100kb message - 2
        // if(!feof(connFp))
        //     fgets(buffer, COPY_BUFFER_MAXSIZE, connFp);

        // write out - 100kb message
//        if(!feof(connFp))
        write(dup(stdoutfd), buffer, (size_t)buffer);
        //fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, stdout); // read(connfd, buffer, (size_t)COPY_BUFFER_MAXSIZE);
        printf("a2\n");
        // fflush(stdout);
        // if(!feof(connFp)){
        //     fputs(buffer,stdout);
        // }
        // print 100kb message - 1
        // if((write(stdinfd, buffer, (size_t)sizeof(buffer)) > 0));
        // print 100kb message - 2
        // for (int i = 0; i < COPY_BUFFER_MAXSIZE; i++)
        //{
        // printf("%c", *(buffer + i));
        //}
        //        for (int i = 0; i < 1; i++)
        //        {
        printf("2\n");
        // write message to client
        //             x = fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
        //   printf("3\n");
        // fflush(outFp);
        //        }
        //    ++i;
        //   break;

        printf("\nEXIT 1\n");
        //        puts("\n");
        // we are on to the next
        //        sha256Checksum(buffer);
        // send messages
        // char buffer2[MSGSIZE];
        // bzero(buffer2, sizeof(buffer2));

        //       printf("EXIT 2\n");
        //        puts("\n");
        // b2) ack to 0b - 100k ie response message
        char buff2[MSGSIZE] = "Message Recieved\n ";
        int n;
        for (int i = 0; i < MSGSIZE; i++)
        {
            // n = read(connfd, temp, sizeof(temp));
            n = write(connfd, buff2, (size_t)sizeof(buff2));
        }

        printf("EXIT 3\n");
        // puts("\n");
        // b3) send sha
        printf("EXIT 4\n");
        //        puts("\n");
        printf("EXIT 5\n");
        //        puts("\n");

    } // END while(true)
    // close up shop
    free(buffer);
    close(connfd);
    // printf("accepting new clients now outside while 1");
    printf("\n\n\nServer work is never done..:)\n\n\n");
    return 0;
} /*            if (read(connfd, buffer, (size_t)sizeof(buffer)) == -1)
             {

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
             }else {
                 break;
             }
 */