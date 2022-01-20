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
    
    if(receive_server_response(socketfd, &welcome_response) != 0 || welcome_response.code != READY_FOR_NEW_USER) {
        printf("Server Response error\n");
        close(socketfd);
        return -1;
    }
    
    printf("Welcome response code: %d\n", welcome_response.code);
    printf("Welcome response description: %s\n", welcome_response.description);
    

    // authenticate the user

    if(authenticate_user(socketfd, args.user, args.pass)) 
    {
        printf("Authentication error\n");
        close(socketfd);
        return -1;
    }
    

    printf("\nSuccessful authentication\n");

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

// reads a string from the server
int receive_server_response_string(int sockfd, char* response){

    char buf[1]; int i=0;

    // reads a byte from the server
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

int send_command(int sockfd, char* command){
    
    // sends a string to the server
    int ret = write(sockfd, command, strlen(command));

    if(ret < 0){
        perror("write");
        return -1;
    }
    
    return 0;
}

int authenticate_user(int socketfd ,char* user, char* pass){

    char user_command[USER_LEN + 10]; 
    char pass_command[PASS_LEN + 10];
    ftp_server_response user_input_response; 
    ftp_server_response pass_input_response;

    // user and password commands to send to the server
    sprintf(user_command,"user %s\n", user);
    sprintf(pass_command,"pass %s\n", pass);


    // sends the user
    if(send_command(socketfd, user_command))
    {
        return -1;
    }

    if(receive_server_response(socketfd, &user_input_response))
    {
        return -1;
    }
 
        
    // Ends after the welcome messages
    while(user_input_response.code == READY_FOR_NEW_USER)
    {
        if(receive_server_response(socketfd, &user_input_response)) 
        {
            return -1;
        }

    }

    // User logged in, proceed
    if(user_input_response.code == USER_LOGIN_OK) return 0;
    
    int num_tries = 0;

    // Tries to receive "User name okay, need password" response MAX_TRIES
    while(user_input_response.code != NEED_PASSWORD && num_tries < MAX_TRIES)
    {        
        printf("\nUnexpected response from server. Trying again in 1 sec...\n");
        sleep(1);

        // sends the user again
        if(send_command(socketfd, user_command))
        {
            return -1;
        }

        if(receive_server_response(socketfd, &user_input_response))
        {
            return -1;
        }
        
        num_tries++;
    }

                                           
    // sends the password
    if(send_command(socketfd, pass_command))
    {
        return -1;
    }

    if(receive_server_response(socketfd, &pass_input_response)) 
    {
        return -1;
    }


    num_tries = 0;
    
    // Tries to receive "User logged in, proceed." response MAX_TRIES
    while(pass_input_response.code != USER_LOGIN_OK && num_tries < MAX_TRIES)
    {   

        printf("\nUnexpected response from server. Trying again in 1 sec...\n");
        sleep(1);

        // sends the pass again
        if(send_command(socketfd, pass_command))
        {
            return -1;
        }

        if(receive_server_response(socketfd, &pass_input_response))
        {
            return -1;
        }
        
        num_tries++;

    }

    if(num_tries == MAX_TRIES)
    {                                   
        printf("\nMaximum tries exceded. Exiting...\n");
        return -1;
    }

    return 0;
}
