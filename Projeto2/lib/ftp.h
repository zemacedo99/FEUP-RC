#ifndef FTP_H
#define FTP_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "macros.h"

int file_transfer(ftp_url args);
int receive_server_response(int sockfd, ftp_server_response* response);
int receive_server_response_string(int sockfd, char* response);
int send_command(int sockfd, char* command);
int authenticate_user(int socketfd ,char* user, char* pass);
int enter_pasv_mode(int socketfd);
int retr_file(int socketfd, int pasv_socketfd, char* path);

#endif 