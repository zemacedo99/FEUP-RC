#include "ftp.h"

int file_transfer(ftp_url args){

    char* ip;
    get_host_ip(args.host, ip);

    int socketfd;
    if((socketfd = connection_TCP(ip, FTP_PORT)) < 0){
        printf("Error in establishing TCP connection\n");
        return -1;
    }
    printf("TCP connection established!\n");


    // TODO: receive welcome response

    // TODO: authenticate the user

    // TODO: Entering Passive Mode

    // TODO: retrieve the file (Download)


    // close the TCP connections
    if (close(socketfd)<0) {
        perror("close(socket)");
        exit(-1);
    }


    return 0;

}