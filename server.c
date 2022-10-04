#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char * argv[])
{
    //print and take in commad line arguments main
    int i;
    for(i = 0; i < argc; i++){
        std::cout << "Argument "<< i << " = " << argv[i] << std::endl;
    }
    return 0;
}
