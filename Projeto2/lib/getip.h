#ifndef GETIP_H
#define GETIP_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @brief get the IP address from hostname
 * 
 * @param host hostname of the server
 * @param ip IP address of the server
 * @return int negative if an error occured. 0 otherwise.
 */
int get_host_ip(char* host, char* ip);

#endif  