#include "../lib/writer.h"


void writer(char *port, char * fileName)
{
  int fd;
  unsigned char package[MAX_SIZE];
  unsigned char fileData[MAX_DATA_SIZE];
  FILE *fp;
  unsigned int seq = 0;
  unsigned int informationMaxSize = MAX_DATA_SIZE;
  unsigned int actualSize = 0;
  unsigned int fileDataSize = 0;

/* open file*/
  fp = fopen(fileName, "rb");
  unsigned int fileSize = get_file_size(fp);

  printf("llopen\n");

  fd = llopen(port, TRANSMITTER);//open file description and data layer


  int size = createControlPackage(START, fileName, fileSize, package); 
  printf("criei control:)\n");
  for (int i = 3; i<3 + strlen(fileName); i++){
        printf("PACKAGE %c\n", package[i]);
    }
  llwrite(fd, package, &size);  //send control package 

  //while true criar packages com numero da sequencia * contentSize e faz-se fread manda-se isso e retorna-se 

  while (0){
      if (seq * informationMaxSize > fileSize){
        actualSize = (seq * informationMaxSize) % fileSize;
      }
      else{
        actualSize = informationMaxSize;
      }
      if (( fileDataSize = fread(fileData, 1, actualSize, fp)) <0){
        break;
      }
      if (createDataPackage(seq, fileDataSize, fileData, package) < 0){
        perror("Failed creating data Package\n");
        return -1;
      }
      if (llwrite(fd, package, &size) == -1){
        return (-1);
      }

      seq++;

  }

    
  size = createControlPackage(END, fileName, fileSize, package); 
  llwrite(fd, package, &size);



  llclose(fd, TRANSMITTER);


  /*
  unsigned char  msg[8] ;
  int lenght = 8;
  msg[0]=0x7e;
  msg[1]=0x63;
  msg[2]=0x7d;
  msg[3]=0x7a;
  msg[4]=0x7d;
  msg[5]=0x7a;
  msg[6]=0x22;
  msg[7]=0x7e;
  
  writeIFrame(fd,msg,lenght);
  */ 


  return;
}

/*
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
  


  fd = llopen(argv[1], TRANSMITTER);

    
  unsigned char  msg[8] ;
  int lenght = 8;
  msg[0]=0x7e;
  msg[1]=0x63;
  msg[2]=0x7d;
  msg[3]=0x7a;
  msg[4]=0x7d;
  msg[5]=0x7a;
  msg[6]=0x22;
  msg[7]=0x7e;
  
  writeIFrame(fd,msg,lenght);
    



  llclose(fd, TRANSMITTER);

  return 0;
}
*/