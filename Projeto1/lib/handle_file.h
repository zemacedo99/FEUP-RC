#ifndef HANDLE_FILE_H
#define HANDLE_FILE_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned long get_file_size(FILE* f);
unsigned char* read_file(FILE* f, unsigned long filesize);
#endif