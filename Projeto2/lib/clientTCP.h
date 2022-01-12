#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * @brief Establish a TCP connection (using SOCKETs) to a server
 * 
 * @param server_ip IP address of the server
 * @param server_port Port of the server, default is FTP_PORT->21
 * @return int negative if an error occured. 0 otherwise.
 */
int connection_TCP(char* server_ip, int server_port);

#endif  