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
void sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy);

int main(int argc, char **argv)
{
    int sockfd;
    int len;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("socketing...\n");
    bzero(&servaddr, sizeof(servaddr));
    int num = atoi(argv[2]);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    int sockfd_dup = dup(sockfd);
    printf("connected...\n");
    // server now has connection
    unsigned char *buffer = NULL;
    buffer = malloc(COPY_BUFFER_MAXSIZE);
    if (!buffer)
        exit(-1);
    //FILE ptr for read from stdin
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


for(int i = 0; i < MSGSIZE; i++){
        fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, inFp);
        fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
}

    // fd struct and fd int for stdout and printing message
    int stdoutfd = fileno(stdout);
    sha256Checksum(buffer);
    printf("\n 1st message sent, anticipating response \n");

        int count2 = 0;
        char buffer2[MSGSIZE];
        bzero(buffer2, sizeof(buffer2));
        FILE *ackFp = fdopen(dup(sockfd),"rb");
        FILE *stdoutFp = stdout;
int n;
for(int i = 0; i < MSGSIZE; i++){
            n = read(sockfd, buffer2, sizeof(buffer2));
}

        fflush(stdoutFp);
        printf("%s", buffer2);   

// recieve sha256 from server

char bufferShaServer[SHA256_DIGEST_LENGTH];
char bufferShaClient[SHA256_DIGEST_LENGTH];
//char bufferShaFinal[SHA256_DIGEST_LENGTH];

for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
            n = read(sockfd, bufferShaServer, sizeof(bufferShaServer));
}


sha256GetChecksum(buffer,bufferShaClient);

if(strncmp(bufferShaServer,bufferShaClient,SHA256_DIGEST_LENGTH)){
   printf("\nSha's from server: %s, from client: %s, they match :)\n", bufferShaServer,bufferShaClient);
}
else{
    printf("\nsha's dont match\n");
}

    
    //    fflush(stdout);
    // get acknoledgement
    char buff2[MSGSIZE];
    // close shop
    fflush(stdoutFp);
    free(buffer);
    close(sockfd);
    return 0;
}
    
//unsigned char * sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy){
void sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy){

           SHA2_CTX ctx;
           uint8_t results[SHA256_DIGEST_LENGTH];
           int n;
           n = strlen(buf);
           SHA256Init(&ctx);
           SHA256Update(&ctx, (uint8_t *)buf, n);
           SHA256Final(results, &ctx);
           
           printf("0x");
           /* Print the digest as one long hex value */
           for (n = 0; n < SHA256_DIGEST_LENGTH; n++){
                   printf("%02x", results[n]);
                   buf_copy[n] = results[n];
           }
           putchar('\n');
}
void sha256Checksum(unsigned char *buf)
{

    SHA2_CTX ctx;
    uint8_t results[SHA256_DIGEST_LENGTH];
    int n;
    n = strlen(buf);
    /*
        The SHA256Init() function initializes a SHA2_CTX context for use with SHA256Update() and SHA256Final().  The SHA256Update() function adds data of length len to the SHA2_CTX
         specified by context.  SHA256Final() is called when all data has been added via SHA256Update() and stores a message digest in the digest parameter.

         The SHA256Transform() function is used by SHA256Update() to hash 512-bit blocks and forms the core of the algorithm.  Most programs should use the interface provided by
         SHA256Init(), SHA256Update(), and SHA256Final() instead of calling SHA256Transform() directly.
    */
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uint8_t *)buf, n);
    SHA256Final(results, &ctx);
    // mine
    //     SHA256Init(*sha256);
    //     SHA256Update(*sha256, *msg, sizeof(msg) - 1);
    printf("0x");
    /* Print the digest as one long hex value */
    for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
        printf("%02x", results[n]);
    putchar('\n');
    //     Alternately, the helper functions could be used in the following way:
    /*
               uint8_t output[SHA256_DIGEST_STRING_LENGTH];
               char *buf = "abc";

               printf("0x%s\n", SHA256Data(buf, strlen(buf), output));
    */
}
