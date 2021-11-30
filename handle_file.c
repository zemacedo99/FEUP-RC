#include "handle_file.h"


unsigned long get_file_size(FILE* f) {
  fseek(f, 0, SEEK_END); 
  unsigned long size = ftell(f); 
  fseek(f, 0, SEEK_SET); 
  return size;
}


//checar 
unsigned char* read_file(FILE* f, unsigned long filesize) {
  unsigned char* data = (unsigned char*) malloc (filesize);
  fread(data, sizeof(unsigned char), filesize, f);
  return data;
}