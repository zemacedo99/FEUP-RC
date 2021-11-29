#include "app.h"

//sudo socat -d  -d  PTY,link=/dev/ttyS0,mode=777   PTY,link=/dev/ttyS1,mode=777

int main(int argc, char** argv)
{
  if ( (argc < 2) || 
        ((strcmp("/dev/ttyS0", argv[1])!=0) && 
        (strcmp("/dev/ttyS1", argv[1])!=0) ) ) {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
    exit(1);
  }
  
  if (argc > 2)
  {
      printf("Emissor\n");
      char * fileName = argv[2];
      writer(argv[1],fileName);
  }
  else
  {
      printf("Recetor\n"); 
      reader(argv[1]);
  }

  return 0;
}