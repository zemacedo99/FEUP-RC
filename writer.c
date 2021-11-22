/*Non-Canonical Input Processing*/

#include "writer.h"




extern int flag, conta;

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    
  int fd;

  if ( (argc < 3) || 
        ((strcmp("/dev/ttyS0", argv[1])!=0) && 
        (strcmp("/dev/ttyS1", argv[1])!=0) ) ) {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
    exit(1);
  }


  char * fileName = argv[2]; 
  
  /* Open serial port device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
  */

  fd = llopen(argv[1], TRANSMITTER);

    



  llclose(fd, TRANSMITTER);

  return 0;
}
