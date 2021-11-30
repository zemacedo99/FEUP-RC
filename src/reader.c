#include "../lib/reader.h"

void reader(char *port)
{
  int fd;

  fd = llopen(port, RECEIVER );

  while (TRUE){
      //llread();
  }



  llclose(fd, RECEIVER);
  return ;
}

/*
#define BAUDRATE B38400
#define _POSIX_SOURCE 1 


unsigned char set[5];
unsigned char ua[5];

int main(int argc, char** argv)
{
    int fd,c, res;


    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

    fd = llopen(argv[1], RECEIVER );
  

  
    llclose(fd, RECEIVER);
    return 0;
}
*/
