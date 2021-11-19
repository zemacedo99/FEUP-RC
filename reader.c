/*Non-Canonical Input Processing*/

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#include "reader.h"

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


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
  
    
    fd = llopen(argv[1], RECEIVER );
    if (fd <0) {perror(argv[1]); exit(-1); }
  

  
    llclose(fd);
    return 0;
}
