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

    FILE *ackFp = fdopen(dup(sockfd), "rb");
    FILE *stdoutFp = stdout;
    char buff2[MSGSIZE];
    bzero(buff2,sizeof(buff2));
    int n = 0;
    do
    {

        fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, inFp);
        fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
        ++count;

//        fread(buffer, 1, (size_t)MSGSIZE, ackFp);
//        fwrite(buffer, 1, (size_t)MSGSIZE, stdoutFp);

        fflush(stdoutFp);
//      while ((n = read(sockfd, buff2, sizeof(buff2))) > 0)
//        {
//            buff2[n] = 0;
//            if (fputs(buff2, stdout) == EOF)
//            {
//                printf("\n fputs:");
//            }
//            fflush(stdoutFp);
//        }
    } while (count < 1);


    // fd struct and fd int for stdout and printing message
    int stdoutfd = fileno(stdout);
    sha256Checksum(buffer);
    printf("\n 1st message sent, anticipating response \n");

    // recieve response
    /*
        int count2 = 0;

        FILE *ackFp = fdopen(dup(sockfd),"rb");
        FILE *stdoutFp = stdout;
        do
        {

            fread(buffer, 1, (size_t)MSGSIZE, ackFp);
            fwrite(buffer, 1, (size_t)MSGSIZE, stdoutFp);
            ++count2;
        } while (count2 < 1);
        fflush(stdoutFp);
    */
    //
    // recieve back from server
    //    fflush(stdout);
    // get acknoledgement
    /*
    int n;
    char buff2[MSGSIZE];
     while((n = read(sockfd, buff2, sizeof(buff2) )) > 0)
    {
        buff2[n] = 0;
        if(fputs(buff2,stdout)==EOF)
        {
            printf("\n fputs:");
        }
        fflush(stdoutFp);
    }
        fflush(stdoutFp);
        printf("From Server : %s", buff2);
        printf("\n\n\nWriting Done..:)\n\n\n");
        printf("\nthe message of %d was sent successfully\n");
    */
//    puts("Ack");
    //
    // close shop
    fflush(stdoutFp);
    free(buffer);
    close(sockfd);
    return 0;
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
    /*
         The SHA256Pad() function can be used to apply padding to the message digest as in SHA256Final(), but the current context can still be used with SHA256Update().
    */
    // mine
    //     SHA256Pad(*sha256);
    /*

         The SHA256End() function is a front end for SHA256Final() which converts the digest into an ASCII representation of the digest in hexadecimal.

         The SHA256File() function calculates the digest for a file and returns the result via SHA256End().  If SHA256File() is unable to open the file, a NULL pointer is returned.

         SHA256FileChunk() behaves like SHA256File() but calculates the digest only for that portion of the file starting at offset and continuing for length bytes or until end of file
         is reached, whichever comes first.  A zero length can be specified to read until end of file.  A negative length or offset will be ignored.

         The SHA256Data() function calculates the digest of an arbitrary string and returns the result via SHA256End().

         For each of the SHA256End(), SHA256File(), SHA256FileChunk(), and SHA256Data() functions the buf parameter should either be a string large enough to hold the resulting digest
         (e.g. SHA256_DIGEST_STRING_LENGTH, SHA384_DIGEST_STRING_LENGTH, or SHA512_DIGEST_STRING_LENGTH, depending on the function being used) or a NULL pointer.  In the latter case,
         space will be dynamically allocated via malloc(3) and should be freed using free(3) when it is no longer needed.
    */
}
