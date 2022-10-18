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
    int sockfd;
    int len;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //    printf("socketing...\n");
    bzero(&servaddr, sizeof(servaddr));
    int num = atoi(argv[2]);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    int sockfd_dup = dup(sockfd);
    //    printf("connected...\n");
    // server now has connection
    unsigned char *buffer = NULL;
    buffer = malloc(COPY_BUFFER_MAXSIZE);
    if (!buffer)
        exit(-1);
    // FILE ptr for read from stdin
    FILE *stdinFp = stdin;
    // set file position indicator for stream pointed to by stream
    //    fseek(inFp, 0, SEEK_END);
    // obtain current value of the file position indicator for the stream pointed to by stream
    //    uint64_t fileSize = ftell(inFp);
    // sets the file position indicator for the stream pointd to by stream to the beginning of the file.
    //    rewind(inFp);

    FILE *sockFp = fdopen(dup(sockfd), "w+");
    int stdinfd = fileno(stdin);
    // b1) 100kb message
    //   for (int i = 0; i < 1; i++)
    //   {
    // read(stdinfd, buffer, (size_t)sizeof(buffer));
    // if (!feof(stdinFp))
    // input length - up to 100kb
    fseek(stdin, 1, SEEK_END);
    size_t stdinLength = ftell(stdin);
    fseek(stdin, 1, SEEK_SET);
    // rewind(stdin);
    // fread(buffer, 1, stdinLength, stdinFp);
    //    }
    //   for (int i = 0; i < 1; i++)
    //   {p
    // read in 100KB message
     if (!feof(stdinFp))
    fread(buffer, 1, (size_t)stdinLength, stdinFp);
    // write out 100kb message
     if (!feof(stdinFp))
    fwrite(buffer, 1, (size_t)stdinLength, sockFp);
    // write(sockfd,buffer,(size_t)sizeof(buffer));
    //
    //   if (!inFp)
    //        {
    //            perror("fwrite");
    //        }
    // x = fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
    //    }
    // fd struct and fd int for stdout and printing message
    // int stdoutfd = fileno(stdout);
    // sha256Checksum(buffer);
    // printf("\n 1st message sent, anticipating response \n");
    printf("sent 100kb mesage\n");
    char buffer2[MSGSIZE];
    bzero(buffer2, sizeof(buffer2));
    FILE *ackFp = fdopen(dup(sockfd), "rb");
    FILE *stdoutFp = stdout;
    // b2) message recieve about recieved message
    int n;
    //    for (int i = 0; i < 1; i++)
    //    {
    read(sockfd, buffer2, (size_t)sizeof(buffer2));
    printf("read ack \n");
    fflush(stdoutFp);

    printf("%s\n", buffer2);

    fflush(stdout);
    // get acknoledgement
    char buff2[MSGSIZE];
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