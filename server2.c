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
 * Problem with ftell() on server side
 * ftell is not giving me the correct size
 * I get -1 when on client this gives correct size
 * If I could get the size of the clients message somehow
 * this would work perfectly
 * I wanted the sleep() to make the server wait long enough to 
 * get the connfd/sockfd(client side/server side) stream full
 * before I tried to get the size, but it is only returning -1
 * functionality not complete,
 * at the moment the client needs a ctrl^c signal
 * to terminate 
 * so basically the length it is measuring isn't long enough
 * 
 * I can make the message print at any size by making the
 * first fread too large of a size
 * but then I need to ctrl-c on client for the server to print the messages
 * 
 * 
 * Server otherwise works fine, however the <EOF>
 * is not terminating the fread() sequence
 * 
 * 
 * 
 * 
 * Otherwise,
 * 
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
 *  
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
#define SA struct sockaddr
#define LISTENQ 5
#define SIZE 1024 * 10
#define COPY_BUFFER_MAXSIZE 1024 * 110 // breathing room...
#define MSGSIZE 30


void sha256GetChecksum(unsigned char *buf, unsigned char *buf_copy);
void sha256Checksum(unsigned char *buf);

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
        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) == NULL)//
        {
            printf("\nMessage from 0.0.0.0\n ");
            printf("\nMessage from  %s:\n ", argv[1]);
            printf("\nMessage from  %s:\n ", buf);
        }else{
          printf("\nMessage from  %s:\n ", buf);
        }
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
            printf("read successful\n");
          
        }
       //print 100kb message 
        printf("\t%s\n", buffer);
//+++++++++++++++++++++++++++++++++++++++++++++    
        //create sha256
        unsigned char * shaBufferServer[50] = {'0'};
        sha256GetChecksum(buffer, shaBufferServer);
        printf("%s\n",shaBufferServer);
//+++++++++++++++++++++++++++++++++++++++++++++  
        char buff2[] = "Message Recieved";
        if(( e = write(connfd, buff2, sizeof(buff2)) == -1 ))
        {
          perror("error second write");//write user message
        } else {
          printf("%s\n", buff2);
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
