#ifndef _DATA_LAYER_H
#define _DATALAYER_H


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>

#include "macros.h"
#include "alarme.h"

int write_sframe(int fd, unsigned char C);

int llopen(char *port, int flag);
int openwriter(char * port);
int recieve_sframe(int fd, unsigned char C);
int llclose(int fd);


#endif