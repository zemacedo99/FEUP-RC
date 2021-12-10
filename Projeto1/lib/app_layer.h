#ifndef APP_LAYER_H
#define APP_LAYER_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "macros.h"

int createControlPackage(unsigned char flag, char* fileName, int fileSize, unsigned char* package );

int createDataPackage(unsigned int seqNum, unsigned int dataSize, unsigned char * data, unsigned char * package);

int readControlPackage(unsigned char * package,  char * fileName, int* fileSize);

int readDataPackage(unsigned char* package, int* seq,unsigned char * data);



#endif