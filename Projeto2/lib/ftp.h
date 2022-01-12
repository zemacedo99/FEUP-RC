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

#endif 