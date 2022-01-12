#include "../lib/clientTCP.h"

int connection_TCP(char* server_ip, int server_port) {

    int sockfd;
    struct sockaddr_in server_addr;
    
    /*server address handling*/
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(server_port);             /*server TCP port must be network byte ordered*/

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }

    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }


    /*send a string to the server*/
    // char buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";
    // size_t bytes;
    // bytes = write(sockfd, buf, strlen(buf));
    // if (bytes > 0)
    //     printf("Bytes escritos %ld\n", bytes);
    // else {
    //     perror("write()");
    //     exit(-1);
    // }

    return 0;
}


