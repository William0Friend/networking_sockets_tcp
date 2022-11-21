/*--------------------------------------------*
 *                                             *
 * Author: William Friend                      *
 * Date: 10/18/2022                            *
 * Server.c                                    *
 * This is the driver for the server.c sockets *
 *  programming exercise.                      *
 *                                             *
 * This follows the standard sequence          *
 * socket                                      *
 * bind                                        *
 * listen                                      *
 * accept                                      *
 *                                             *
 * Server runs an infinate loop reading from   *
 * clients and recieving messages of up to     *
 * 100kb                                       *
 *                                             *
 * Server then sends a client a response       *
 *                                             *
 * Server's session runs inifinately
 * 
 *
 * IMPORTANT:
 * 
 *Ok, just fyi I don't expect this to change my grade.

I just wanted to show you I didn't give up on this yet, and it has been bugging me that it didn't work. I handed in a really stripped down version to hit the due date. But to do this, I had a lot of c library stuff to read.

I still have to figure out how to get the client ip from the struct and print it, but otherwise, the non extra credit part works now, I figured out the read write problem, and the sha works with the sample file of under 10kb but not my 20kb and higher. Ill keep working on it whenever I get time.

All the experiments i've been doing to try and figure c tcp servers out is at this github repo:


https://github.com/William0Friend/networking_sockets_tcp  
 * 
 * 
 * 
 * 
 *---------------------------------------------**/


// Libraries needed
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sha2.h>
#include <errno.h>
#include <error.h>
#include <assert.h>

#define SA struct sockaddr
#define LISTENQ 5
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 110 // breathing room...
#define MSGSIZE 30


void sha256GetChecksum(unsigned char *buf, uint8_t * buf_copy);
void sha256Checksum(unsigned char *buf);
void printSha256(uint8_t * results);
// Driver code
int main(int argc, char **argv)
{
    int e, listenfd, connfd;// file descriptors
    socklen_t client;//client ip address
    struct in_addr addr;//copy of ip address
    struct sockaddr_in servaddr, cliaddr;//create stuct
    char* buffer =  malloc(COPY_BUFFER_MAXSIZE);
    
    int sizeBuffer = COPY_BUFFER_MAXSIZE; 
    bzero(buffer, sizeBuffer);
//+++++++++++++++++++++++++++++++++++++++++++++    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
      perror("Error while creatin the socket\n");// socket file descriptor
    }

//+++++++++++++++++++++++++++++++++++++++++++++    
    bzero(&servaddr, sizeof(servaddr));//zero out our memory
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//assign ip, with correct endianess
    servaddr.sin_port = htons(atoi(argv[1])); //assign port, with correct endianess
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);//convert port number to ipv4
    //+++++++++++++++++++++++++++++++++++++++++++++    
    if(bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        perror("error with bind \n");// bind to the socket
//+++++++++++++++++++++++++++++++++++++++++:++++    
   if(listen(listenfd, LISTENQ) < 0)
    perror("error with listen\n");// start listening for new clients
//+++++++++++++++++++++++++++++++++++++++++++++    
    while (1)// infinite server loop, ctrl-c to terminate
    {
//+++++++++++++++++++++++++++++++++++++++++++++    
        connfd = accept(listenfd, (SA *)&cliaddr, &client);
        if(connfd < 0)
        {
          perror("error with accept");// create file pointer to easily write large amounts over socket
        }
          //+++++++++++++++++++++++++++++++++++++++++++++    
        
        addr.s_addr = cliaddr.sin_addr.s_addr;
        //get clients address from struct
        char * buf = argv[1];
        
       // printf("Client Address = %s\n", inet_ntop(AF_INET, &cliaddr.sin_addr, cliaddr.sin_addr.s_addr,sizeof(cliaddr.sin_addr.s_addr));
        printf("\nMessage from 0.0.0.0\n ");
       /*
        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) == NULL)//
        {
            printf("\nMessage from 0.0.0.0\n ");
            printf("\nMessage from  %s:\n ", argv[1]);
            printf("\nMessage from  %s:\n ", buf);
        }else{
          printf("\nMessage from  %s:\n ", buf);
        }
        */
        //+++++++++++++++++++++++++++++++++++++++++++++
        int stdoutFp = fileno(stdout);
        int count = 0;
        int f = 0;
        //printf("printf %s\n", buffer);
        
        e = read(connfd, buffer, --sizeBuffer); 
        if(e == EOF)
        {
            perror("error first read\n");
        } else {
         //   printf("read successful\n");
          printf("\n");
        }
       //print 100kb message 
        printf("\t%s\n", buffer);
//+++++++++++++++++++++++++++++++++++++++++++++    
        //create sha256
        uint8_t shaBufferServer[SHA256_DIGEST_STRING_LENGTH];
        sha256GetChecksum(buffer, shaBufferServer);
//        printf("%s\n",shaBufferServer);
//         printf("\n");//print the message
//         printSha256(shaBufferServer);
//+++++++++++++++++++++++++++++++++++++++++++++  
        char buff2[] = "Message Recieved";

        if(( e = write(connfd, buff2, sizeof(buff2)) == -1 ))
        {
          perror("error second write");//write user message
        } else {
//          printf("%s\n", buff2);
          printf("\n");
        }
//+++++++++++++++++++++++++++++++++++++++++++++    
//+++++++++++++++++++++++++++++++++++++++++++++  
        //unsigned char * shaBufferServer[50] = {'0'};
        // send sha to server then print
        if(( e = write(connfd, shaBufferServer, sizeof(shaBufferServer)) == -1 ))
        {
          perror("error sha write");//write user message
        } else {
          //assert(shaBufferServer[0] == shaBufferClient[0]);
          printf("Server Version of Client Sha:\n");//print the message
          printSha256(shaBufferServer);
          printf("\n");//print the message
        }
//+++++++++++++++++++++++++++++++++++++++++++++    
        //printf("Sent Message recieved to client\n\0");
        printf("\n\n\n\n\nAccepting new clients..\n\0");
    } // END while(true)
    
    // close up shop
    free(buffer);
    close(connfd);
    printf("\n\n\nServer work is never done...\n\n\n\0");
    return 0;
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
// unsigned char * sha256GetChecksum(unsigned char * buf, unsigned char * buf_copy){
void sha256GetChecksum(unsigned char * buf, uint8_t * buf_copy)
{

    SHA2_CTX ctx;
    uint8_t results[SHA256_DIGEST_LENGTH];
    int n;
    n = strlen(buf);
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uint8_t *)buf, n);
    SHA256Final(results, &ctx);
    
     //printf("0x");
     for (n = 0; n < SHA256_DIGEST_LENGTH; n++){
         //    printf("%02x", results[n]);
             buf_copy[n] = results[n];
             assert(buf_copy[n] == results[n]);
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

        //e = 1; 
        /*
        while ((e = read(connfd, &buffer, sizeof(char) )) != 0)
            {
              if(e == -1)
              {
                perror("bad read\n");
              }
                write(STDOUT_FILENO, buffer, sizeBuffer);  
                  //if(e == 0) 
                  //  break;
            }
                //if(e == -1)
                //perror("bad write\n");
            //if(e == sizeBuffer)
            //{ 
              //  goto next; 
            //}
            printf("E: %d \n", e);
        //printf("%s\n", buffer);
        */

        /*
        while( ( e = read(connfd, buffer, sizeof(buffer)) != EOF ) )
        {
            
            write(stdoutFp, buffer, sizeof(buffer));
            //printf("\n\tcount: %d\n", count);
            if( f == (sizeof(buffer)-1) )
            {
              //printf("\n\tcount: %d\n", count);
              break;
            }
        }
        */   

        //printf("%s\n", buffer);
        //
        /*write replaced with above printf
        
        e = write(stdoutFp, &buffer, --sizeBuffer);
        if(e == EOF){
            perror("bad write\n");
        }else{
        printf("write succesful\n");
        }
        //if(( e = write(stdoutFp, buffer, sizeof(buffer)) == -1))
        //{    
        //    perror("error first write");
        //}else{
        //    //printf("%s\n", buffer);
        //}
        */
