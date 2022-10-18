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
void sha256Checksum(unsigned char * buf);
unsigned char * sha256GetChecksum(unsigned char *, unsigned char *);


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

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //    printf("socketing...\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    //    printf("binded...\n");

    listen(listenfd, LISTENQ);
    //    printf("listening...\n");
    client = sizeof(cliaddr);

    while (1)
    {
        connfd = accept(listenfd, (SA *)&cliaddr, &client);
        // printf("accepted client...\n");
        addr.s_addr = cliaddr.sin_addr.s_addr;
       // const char *inet_ntop(int af, const void *restrict src, char *restrict dst, socklen_t size);
       //           printf("inet_net_pton() returned: %d\n", bits);

        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) == NULL)
        {
            printf("\nMessage from 0.0.0.0\n ");
        }
        printf("\nMessage from  %s:\n ", buf);

        // server now has connection
        // print Message up to 100-110kb
        //  assign memory for the message.
        unsigned char *buffer = NULL;
        buffer = malloc(COPY_BUFFER_MAXSIZE);
        if (!buffer)
            exit(-1);
        // FILE *inFp = stdin;
        // 2
        FILE *inFp = fdopen(dup(connfd), "rb"); // set file position indicator for stream pointed to by stream
        // set pointer to begining of file for sure
        FILE *outFp = stdout; // we want our output file to be stdout
                              //        uint64_t outFileSizeCounter = fileSize;
        int count = 0;
        char buff2[MSGSIZE] = "\nMessage Recieved\n ";
        
        do
        {
//read write 100k message
        fread(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, inFp);
        fwrite(buffer, 1, (size_t)COPY_BUFFER_MAXSIZE, outFp);
            ++count;
        //fflush(outFp);
        // we are on to the next
//print and send ack
        //snprintf(buff2, sizeof(buff2), "Message Recieved inside ");
        //write(connfd, buff2, strlen(buff2));
        //sleep(1);
            // printf("\nafter fwrite\n ");
            // printf("\nno if count = %d\n ", count);
          /*
            char buff2[MSGSIZE];
            int count2 = 0;
            while(count2 < 1){
                snprintf(buff2, sizeof(buff2), "Message Recieved inside ");
                write(connfd, buff2, strlen(buff2));
                sleep(1);
            }
        */
//print checksum
        //sha256Checksum(buffer);
        //printf("\naccepting new clients now...\n\n");
        //fflush(outFp);
         /*   
        //send messages    
            char buff2[MSGSIZE];
            int count2 = 0;
            while(count2 < 1){
                snprintf(buff2, sizeof(buff2), "Message Recieved inside ");
                write(connfd, buff2, strlen(buff2));
                sleep(1);
            }
        */    
    }while(count < 1); // end do...while()


        // close up shop
        free(buffer);
        close(connfd);
        sleep(1);
        // printf("accepting new clients now 0utside while 1");

    printf("\n\n\nServer work is never done..:)\n\n\n");
    return 0;
        /*
        char ack[MSGSIZE] = "Message Recieved\n";s
        printf("%s",ack);
        fflush(outFp);
        sendto(connfd,ack,sizeof(ack),0,NULL,0);
        printf("Ack Sent\n");
        */
        fflush(outFp);
//send response 
/*
        int connfd_dup = dup(listenfd);
            char buff2[MSGSIZE];
            int count2 = 0;
            while(count2 < 1){
                snprintf(buff2, sizeof(buff2), "Message Recieved\n ");
                write(connfd_dup, buff2, strlen(buff2));
                sleep(1);
                ++count;
            }
*/
//send checksum
//not working
/*
        char buff2[MSGSIZE] = {'M', 'E', 'S', 'S', 'A', 'G', 'E', ' ', 'R', 'e', 'c', 'i', 'e', 'v', 'e', 'd', '\0'};
        
        while(1){
            snprintf(buff2,sizeof(buff2),"Message Recieved outside");
            write(connfd, buff2, strlen(buff2));
        }
*/
        // we are on to the next
        printf("\naccepting new clients now...\n\n");
        sha256Checksum(buffer);
        fflush(outFp);
         /*   
        //send messages    
            char buff2[MSGSIZE];
            int count2 = 0;
            while(count2 < 1){
                snprintf(buff2, sizeof(buff2), "Message Recieved inside ");
                write(connfd, buff2, strlen(buff2));
                sleep(1);
            }
        */    
        // close up shop
        free(buffer);
        close(connfd);
        sleep(1);
        // printf("accepting new clients now 0utside while 1");

    } // end while(1)
      // 3
    printf("\n\n\nServer work is never done..:)\n\n\n");
    return 0;
}
unsigned char * sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy){

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
//mine
//    SHA256Init(*sha256);
//    SHA256Update(*sha256, *msg, sizeof(msg) - 1);
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
//mine
//    SHA256Pad(*sha256);
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
void sha256Checksum(unsigned char * buf){

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
//mine
//    SHA256Init(*sha256);
//    SHA256Update(*sha256, *msg, sizeof(msg) - 1);
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
//mine
//    SHA256Pad(*sha256);
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





