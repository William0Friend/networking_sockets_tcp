#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char * argv[])
{
    //print and take in commad line arguments main
    int i;
    for(i = 0; i < argc; i++){
        std::cout << "Argument "<< i << " = " << argv[i] << std::endl;
    }
  1. You will implement a simple command-line oriented messaging client and server (no GUIs, no
dialog boxes, no graphics). The server will be invoked (from the command line) thus:
server <port-number>
Where <port-number> is any numeric port number that the user provides at run time via
the command line. The server attempts to bind a TCP socket to this port and listen
for connections (if the server cannot bind to this port, the server will exit after printing an
informative error message for the user). The client will be invoked as follows:
client <server-address> <server-port-number>
Here, <server-address> is the IP address of the server and <server-port-number> is the port
number for the TCP connection on the server. The IP address and port number are provided
at run time via the command line.
2. The client will read from standard input and transmit all characters, up to the <EOF> indicator
(in Linux, this would be control-d, in windows, it is usually control-z).

3. When the server receives the text message, it will display the message to standard output
(including the IP address of the machine that sent the message), and send an acknowledgement
back to the client in the form of a text message: “Message received.”
4. The client and server will then drop the TCP connection and the server will continue listening
for new connections.
5. You must implement your assignment such that it meets the following requirements:
• Implemented in the C programming language1
• Compiles with the GCC or Clang C compiler
• Is POSIX compliant code, and uses the sys/socket (and perhaps the sys/types) libraries
6. You must submit your complete source code, with a block-style header comment which includes
at the very least, your name (or names for undergrads that worked in pairs), our class number,
and the date.
7. Make sure that your code is well documented and uses appropriate naming conventions for all
variables, procedures, methods, and classes (as appropriate). If your code is not well documented
and does not follow appropriate naming conventions, you will lose points.
8. Test your code. You will submit a checklist with your code that indicates if your project compiles
or not, if it runs or not, and if it runs with errors. Be honest. If you are not honest on your
checklist, you will lose points.
9. Your client/server must be able to handle a message of arbitrary size, from small to rather large
(somewhere around 100KB). When I test your program, I will redirect the contents of a text file
to the standard input of the client. The max file size that I will use will be around 100K.
10. Make sure you read from standard input and display the message result to standard output.
There is no need for any file I/O in this assignment.
11. If your server can not bind to the port given on the command line, it should display an
appropriate message to standard error and then gracefully cleanup/exit
12. If the client is unable to bind/connect with the server, it should also display an appropriate
message to standard error and then gracefully cleanup/exit.
13. Note well: I am keenly aware that many examples of simple TCP client/server messaging code
exist on the Internet. Don’t sell yourself short. Write the code yourself and learn more about
TCP and sockets. Your future boss will like you for it. Plus, I use a rather sophisticated
plagiarism detection tool (not the stock D2L plagiarism essay checker). I would hate to have to
send business to the student conduct board at this stage of the game.
14. In the example below, the server is running on a machine with IP address 192.168.1.2 and the
client is on a machine with IP address 192.168.1.4. You will provide your own message (test withmessages of various length), IP address, and port number. You will not type the five characters
“<EOF>” but type the single <EOF> character.2
Example input/output for the client is shown below. Output is in blue and input is in red (the
format of your input/output must comply with the example below):
client 192.168.1.2 62637
Some interesting and exciting message.<EOF>
Message received.
Example input/output for the server is shown below. Output is in blue and input is in red (the
format of your input/output must comply with the example below):
server 62637
Message from 192.168.1.4:
Some interesting and exciting message.





    return 0;
}
