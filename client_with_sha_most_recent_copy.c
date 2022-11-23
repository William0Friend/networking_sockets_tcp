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
 *Ok, just fyi I don't expect this to change my grade.

I just wanted to show you I didn't give up on this yet, and it has been bugging me that it didn't work. I handed in a really stripped down version to hit the due date. But to do this, I had a lot of c library stuff to read.

I still have to figure out how to get the client ip from the struct and print it, but otherwise, the non extra credit part works now, I figured out the read write problem, and the sha works with the sample file of under 10kb but not my 20kb and higher. Ill keep working on it whenever I get time.

All the experiments i've been doing to try and figure c tcp servers out is at this github repo:


https://github.com/William0Friend/networking_sockets_tcp  
 *---------------------------------------------*/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sha2.h>
#include <errno.h>
#define SA struct sockaddr
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 100
#define MSGSIZE 19

void sha256GetChecksum(unsigned char *buf, unsigned char buf_copy[]);
void sha256Checksum(unsigned char *buf);
void printSha256(uint8_t * results);

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
    printf("From Client: \n");
    //printf("%s\n", shaBufferClient);
    printSha256(shaBufferClient);
    printf("\n");
//+++++++++++++++++++++++++++++++++++++++++++++    
//get checksum from server print it
    
    unsigned char shaBufferServer[50] = {'0'};
    if((e = read(sockfd, shaBufferServer, (size_t)sizeof(shaBufferServer)) == -1))
    {
        perror("second read no good");//read "Message Recieved from server"
    }else{
        //printf("%s\n", shaBufferServer);
        assert(shaBufferServer[0] == shaBufferClient[0]);
        printf("From Server: \n");
        printSha256(shaBufferServer);
        printf("\n");
    }
//+++++++++++++++++++++++++++++++++++++++++++++    
    //compare checksums
    e = strcmp(shaBufferClient, shaBufferServer);
    if(e < 0 || e > 0)
    {
        printf("They don't match!\n");
    }else{
        printf("They match!\n");
    }
        //shaCheck(shaBufferClient, shaBufferServer)
        //printf("They match!");
        //else
        //printf(They don't match!");
//+++++++++++++++++++++++++++++++++++++++++++++    
    //print whether checksums match
//+++++++++++++++++++++++++++++++++++++++++++++    
    // close shop
    free(buffer);
    close(sockfd);
    return 0;
}


// unsigned char * sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy){
void sha256GetChecksum(unsigned char *buf, unsigned char * buf_copy)
{

    SHA2_CTX ctx;
    uint8_t results[SHA256_DIGEST_LENGTH];
    int n;
    n = strlen(buf);
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uint8_t *)buf, n);
    SHA256Final(results, &ctx);
    /*
     for (int i = 0; i < SHA256_DIGEST_LENGTH; i++;)
     {
            buf_copy[i] = results[i];
     }
     */

   // printf("0x");
     for (n = 0; n < SHA256_DIGEST_LENGTH; n++){
   //          printf("%02x", results[n]);
             buf_copy[n] = results[n];
             assert(buf_copy[n] == results[n]);
     }
     putchar('\n');
}

void printSha256(uint8_t * results)
{
     int n;
     printf("0x");
     for (n = 0; n < SHA256_DIGEST_LENGTH; n++){
             printf("%02x", results[n]);
           //  buf_copy[n] = results[n];
     }
     putchar('\n');

}

