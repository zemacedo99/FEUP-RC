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
    
    // printf("Welcome response code: %d\n", welcome_response.code);
    // printf("Welcome response description: %s\n", welcome_response.description);
    

    // authenticate the user

    if(authenticate_user(socketfd, args.user, args.pass)) 
    {
        printf("Authentication error\n");
        close(socketfd);
        return -1;
    }
    
    printf("\nSuccessful authentication\n");

    // entering passive mode

    int pasv_socketfd = enter_pasv_mode(socketfd);
    if(pasv_socketfd < 0)
    {
        printf("Passive Mode error\n");
        close(socketfd);
        return -1;
    }

    printf("\nSuccessful Passive Mode\n");

    // retrieve the file (Download)

    if(retr_file(socketfd, pasv_socketfd, args.path)){
        printf("Retrieving File error\n");
        close(socketfd);
        close(pasv_socketfd);
        return -1;    
    }

    printf("\nSuccessful Retrieved File\n");

    // close the TCP connections
    if (close(socketfd)<0 || close(pasv_socketfd)<0) {
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

    printf("\nString send to the server:\n%s\n", command);
    
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

int enter_pasv_mode(int socketfd){

    ftp_server_response pasv_response;

    // send pasv command to enter passive mode
    if(send_command(socketfd, "pasv \n")) 
    {
        return -1;
    }

    if(receive_server_response(socketfd, &pasv_response)) 
    {
        return -1;
    }

    // printf("pasv response code: %d\n", pasv_response.code);
    // printf("pasv response description: %s\n", pasv_response.description);

    int num_tries = 0;
    
    // Tries to receive "Entering Passive Mode (h1,h2,h3,h4,p1,p2)." response MAX_TRIES
    while(pasv_response.code != ENTER_PASV_MODE && num_tries < MAX_TRIES)
    {
        printf("\nUnexpected response from server. Trying again in 1 sec...\n");
        sleep(1);

        if(send_command(socketfd, "pasv \n")) 
        {
            return -1;
        }

        if(receive_server_response(socketfd, &pasv_response)) 
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


    strtok(pasv_response.description, "(");

    char *six_bytes = strtok(NULL, ")");

    IPv4_address server_ip; 

    int p1, p2;
    char ip[30];


    // parse the string into the IP and ports

    sscanf(six_bytes,
            "%d,%d,%d,%d,%d,%d",
            &server_ip.h1,
            &server_ip.h2,
            &server_ip.h3,
            &server_ip.h4,
            &p1,
            &p2);
    
    sprintf(ip,
            "%d.%d.%d.%d",
            server_ip.h1,
            server_ip.h2,
            server_ip.h3,
            server_ip.h4);

    // calculate the port on the server that is waiting for a connection
    int port = p1 * 256 + p2;    


    int pasv_socketfd;
    if((pasv_socketfd = connection_TCP(ip, port)) < 0){
        printf("Error in establishing TCP connection with server to transfer data in passive mode\n");
        return -1;
    }

    printf("TCP connection established with server to transfer data in passive mode\n");

    return pasv_socketfd;
}

int retr_file(int socketfd, int pasv_socketfd, char* path){

    char retr_command[PATH_LEN + 10]; 
    ftp_server_response retr_response;
    
    // retr command to send to the server
    sprintf(retr_command, "retr %s\n", path);

    // Sending retrieve command
    if(send_command(socketfd, retr_command))
    {
        return -1;
    }

    if(receive_server_response(socketfd, &retr_response)) 
    {
        return -1;
    }


    int num_tries = 0;
    
    // Tries to receive "File status okay; about to open data connection." response MAX_TRIES
    while(retr_response.code != FILE_STATUS_OK && num_tries < MAX_TRIES){
        printf("\nUnexpected response from server. Trying again in 1 sec...\n");
        sleep(1);

        if(send_command(socketfd, retr_command)) 
        {
            return -1;
        }

        if(receive_server_response(socketfd, &retr_response)) 
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

    char filename[FILENAME_LEN];
    if(parse_filename(path, filename))
    {
        return -1;
    }

    /*
        flag -> 0644
        (owning) User: read & write
        Group: read
        Other: read

        O_WRONLY ->  write-only
        O_CREAT -> If pathname does not exist, create it as a regular file.
        
    */
    // open file descriptor
    int filefd = open(filename, O_WRONLY | O_CREAT, 0644);
    if(filefd == -1){
        perror("open file descriptor");
        return -1;
    }
    
    char buf[1]; 
    int rd, wt;

    do{
        // read from the server socker
        rd = read(pasv_socketfd, buf, 1);
        if(rd == -1){
            perror("read");
            return -1;
        }

        // write on file
        wt = write(filefd, buf, 1);
        if(wt == -1){
            perror("write");
            return -1;
        }
    }while(rd != 0);

    close(filefd);

    ftp_server_response transfer_ok_response;

    if(receive_server_response(socketfd, &transfer_ok_response) != 0 || transfer_ok_response.code != TRANSFER_COMPLETE) 
    {
        return -1;
    }
    
    return 0;
}
