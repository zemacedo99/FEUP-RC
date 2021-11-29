/*Non-Canonical Input Processing*/

#include "writer.h"


extern int flag, conta;

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    
  int fd;
  unsigned char package[MAX_SIZE];  
  FILE *fp;
  int seq = 0;
  
  if ( (argc < 3) || 
        ((strcmp("/dev/ttyS0", argv[1])!=0) && 
        (strcmp("/dev/ttyS1", argv[1])!=0) ) ) {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
    exit(1);
  }


/* open file*/
  char * fileName = argv[2]; 
  fp = fopen(fileName, "rb");
  unsigned int fileSize = get_file_size(fp);


  //fd = llopen(argv[1], TRANSMITTER);//open file description and data layer


  int size = createControlPackage(START, fileName, fileSize, package); 
  printf("criei control:)\n");
  for (int i = 3; i<3 + strlen(fileName); i++){
        printf("PACKAGE %c\n", package[i]);
    }
  //llwrite(fd, package, &size);  //send control package 

  //while true criar packages com numero da sequencia * contentSize e faz-se fread manda-se isso e retorna-se 

  while (TRUE){


  }


  




  /* Open serial port device for reading and writing and not as controlling tty
  because we don't want to get killed if linenoise sends CTRL-C.
  */



    



  //llclose(fd, TRANSMITTER);

  return 0;
}
