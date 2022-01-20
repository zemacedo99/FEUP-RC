#include "../lib/ftp.h"

int file_transfer(ftp_url args){

    char ip[30];
    get_host_ip(args.host, ip);

    int socketfd;
    if((socketfd = connection_TCP(ip, FTP_PORT)) < 0){
        printf("Error in establishing TCP connection\n");
        return -1;
    }
    printf("TCP connection established!\n");


    ftp_server_response welcome_response;

    // receive welcome response
    receive_server_response(socketfd, &welcome_response);

    printf(welcome_response.description);

    /*
    if(receive_server_response(socketfd, &welcome_response) != 0 || welcome_response.code != 220) {
        close(socketfd);
        return -1;
    }
    */
    

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

int receive_server_response(int sockfd, ftp_server_response* response){

    char response_string[SERVER_RESPONSE_LEN];

    if(receive_server_response_string(sockfd, response_string))
    {
        return -1;
    } 
    
    printf("\nString received by server:\n%s\n", response_string);
    
    
    if(parse_server_response(response_string, response, " ") != 0 && parse_server_response(response_string, response, "-") != 0) 
    {
        return -1;
    }


    return 0;
}

int receive_server_response_string(int sockfd, char* response){

    char buf[1]; int i=0;

    int ret = read(sockfd, buf, 1);

    if(ret < 0){                        
        perror("read");
        return -1;
    }

    response[i++] = buf[0];

    // ends when it reads \n
    while(buf[0] != '\n')
    {              
        ret = read(sockfd, buf, 1);
        if(ret < 0){
            perror("read");
            return -1;
        }
        else if(ret == 0) break;

        response[i++] = buf[0];
    }

    response[i] = '\0';

    return 0;
}