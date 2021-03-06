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

#include "alarme.h"

int writeIFrame(int fd, unsigned char *msg,  int lenght);

int writeFrame(int fd, unsigned char A, unsigned char C);

int receiveIFrame(int fd, unsigned char *buffer);

int receiveRFrame(int fd);

int llopen(char *port, unsigned char flag);

int openPort(char * port);

int recieveSFrame(int fd, unsigned char A, unsigned char C);

int llclose(int fd, unsigned char flag);

void activateAlarm();

void desactivateAlarm();

int receiveIData(int fd, unsigned char  bcc1, unsigned char  *buffer);

int llwrite(int fd, unsigned char * buffer, int length);

int llread(int fd, unsigned char * buffer);

void updateSenderN();

void updateRecieverN();

#endif