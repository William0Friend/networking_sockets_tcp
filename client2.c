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
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sha2.h>
#include <errno.h>
#define SA struct sockaddr
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 100
#define MSGSIZE 19

void sha256GetChecksum(unsigned char *buf, unsigned char *buf_copy);
void sha256Checksum(unsigned char *buf);

int main(int argc, char **argv)
{   
    int e, sockfd;// file descriptors
    int len;
    struct sockaddr_in servaddr, cliaddr;
    //create stuct
    
    fflush(stdout);
//+++++++++++++++++++++++++++++++++++++++++++++    
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("opening stream socket");
  }
//+++++++++++++++++++++++++++++++++++++++++++++    
    bzero(&servaddr, sizeof(servaddr));//zero out our memory
    int num = atoi(argv[2]);//take in port number
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//assign ip, with correct endianess
    servaddr.sin_port = htons(atoi(argv[2])); //assign port, with correct endianess
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);//convert port number to ipv4
//if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
//  err_n_die("inet_pton error for %s ", argv[1]
//+++++++++++++++++++++++++++++++++++++++++++++    
  
  int sizeBuffer = COPY_BUFFER_MAXSIZE;
  
  e = connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
  if(e < 0){
    perror("connection error");
  }
   //conect to the server
//+++++++++++++++++++++++++++++++++++++++++++++    
//+++++++++++++++++++++++++++++++++++++++++++++    `
    char *buffer = malloc(COPY_BUFFER_MAXSIZE);
    bzero(buffer, sizeBuffer);
    //printf("sent buffer, used %db out of possible 100kb\n",fileSize);
//+++++++++++++++++++++++++++++++++++++    
    
    int stdinfd = fileno(stdin);
    
    //read in message 
    read(stdinfd, buffer,(size_t)(sizeBuffer-1));
    //write out message
    write(sockfd, buffer, (size_t)(sizeBuffer-1));
    if( e == -1){
        perror("write error()");
    }
//+++++++++++++++++++++++++++++++++++++++++++++    
    //printf("sent 100kb mesage\n");
    char buffer2[MSGSIZE];
    bzero(buffer2, sizeof(buffer2));
    if((e = read(sockfd, buffer2, (size_t)sizeof(buffer2)) == -1))
    {
        perror("second read no good");//read "Message Recieved from server"
    }
    printf("%s\n", buffer2);//print the message
//+++++++++++++++++++++++++++++++++++++++++++++    
    //get checksum
    unsigned char shaBufferClient[50] = {'0'};
    sha256GetChecksum(buffer, shaBufferClient);
    printf("%s\n", shaBufferClient);

//+++++++++++++++++++++++++++++++++++++++++++++    
    //get checksum from server
    /*
    unsigned char bufferSha[SHA256_DIGEST_STRING_LENGTH];
    bzero(bufferSha, sizeof(bufferSha));
    if((e = read(sockfd, bufferSha, (size_t)sizeof(buffer2)) == -1))
    {
        perror("second read no good");//read "Message Recieved from server"
    }
    */
//+++++++++++++++++++++++++++++++++++++++++++++    
    //compare checksums
//+++++++++++++++++++++++++++++++++++++++++++++    
    //print whether checksums match
//+++++++++++++++++++++++++++++++++++++++++++++    
    // close shop
    free(buffer);
    close(sockfd);
    return 0;
}


// unsigned char * sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy){
void sha256GetChecksum(unsigned char *buf, unsigned char *buf_copy)
{

    SHA2_CTX ctx;
    uint8_t results[SHA256_DIGEST_LENGTH];
    int n;
    n = strlen(buf);
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uint8_t *)buf, n);
    SHA256Final(results, &ctx);
    
     printf("0x");
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
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uint8_t *)buf, n);
    SHA256Final(results, &ctx);
    
    printf("0x");
    for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
        printf("%02x", results[n]);
    putchar('\n');
    
    //     Alternately, the helper functions could be used in the following way:
    
    //           uint8_t output[SHA256_DIGEST_STRING_LENGTH];
    //           char *buf = "abc";

    //           printf("0x%s\n", SHA256Data(buf, strlen(buf), output));
    
}

   /* 
     e = 1;
    while((e = read(STDIN_FILENO, &buffer, sizeof(char))) != 0); 
    {
      if(e == -1)
      {
        perror("bad read\n");
      }
        write(sockfd, &buffer, sizeof(char));
        //if(e == 0)
        //  break;
    }
    */
