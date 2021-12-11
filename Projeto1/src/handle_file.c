#include "../lib/handle_file.h"


unsigned long get_file_size(FILE* f) {
  fseek(f, 0, SEEK_END); 
  unsigned long size = ftell(f); 
  fseek(f, 0, SEEK_SET); 
  return size;
}

