#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sha2.h>
#define SA struct sockaddr
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 100
#define MSGSIZE 19
void sha256Checksum(unsigned char *);
void sha256GetChecksum(unsigned char *buf, unsigned char *buf_copy);

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
    FILE *inFp = stdin;
    // set file position indicator for stream pointed to by stream
    fseek(inFp, 0, SEEK_END);
    // obtain current value of the file position indicator for the stream pointed to by stream
    uint64_t fileSize = ftell(inFp);
    // sets the file position indicator for the stream pointd to by stream to the beginning of the file.
    rewind(inFp);

    FILE *outFp = fdopen(dup(sockfd), "wb");
    uint64_t outFileSizeCounter = fileSize;
    int count = 0;

    // b1) 100kb message
    int x = 0;
    for (int i = 0; i < 1; i++)
    {
        x = fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, inFp);
        //fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
    }
    
    x = 0;
    for (int i = 0; i < 1; i++)
    {
        //fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, inFp);
        x = fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
    }

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
    for (int i = 0; i < MSGSIZE; i++)
    {
        n = read(sockfd, buffer2, sizeof(buffer2));
    }

    //fflush(stdoutFp);
    
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