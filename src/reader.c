#include "../lib/reader.h"

void reader(char *port)
{
  int fd;int res;
  int seq = 0;
  char fileName[256];
  FILE *fp;
  int length; int fileSize;
  unsigned char package[MAX_PACKAGE_SIZE];
  unsigned char data[MAX_DATA];


  
  fd = llopen(port, RECEIVER );

  printf("AQUI LOGO DEPOIS DO LLOPEN\n");


  //recieve control package

  while (TRUE){
    res = llread(fd, package);
    printf("sai do ll read\n");
    if (package[0]==START){
      if (readControlPackage(package, fileName, &fileSize, res)>0)
          break;
    }
  }

  printf("LI CONTROL PACKAGE FIXE\n");
  printf( "file size %d\n", fileSize);

  printf("vou chamar o pinguim %s\n", fileName);
  

  if( (fp = fopen(fileName, "wb")) == NULL ) {
        perror("Failed creating file\n");
        exit(-1);
    } 
    

  while (TRUE){
      if ( (length = llread(fd,package)) < 0){
            perror("Could not read file descriptor.");
            exit(-1);
        }
      

      if (package[0]== DATA){
        if (res = readDataPackage(package, &seq, data)<0){
          exit (-1);
        };
        for (int i = 0;i< length; i++){
        printf( " %d estou a receber %x   ", seq, package[i]);
        }
        
        fwrite(data, 1, length, fp); 
        printf("AQUI LOGO DEPOIS DO READ PACKAGE %d\n", seq);


      }

      else if (package[0] == END){
        char endFileName[FILENAME_MAXSIZE];
        int endSize;

        if (readControlPackage(package, &endFileName, &endSize, length)>0){
            if (strcmp(endFileName, fileName) != 0){
                perror("Incompatible information recieved\n");
                exit (-1);
            }
            if (fileSize != endSize){
                perror("Incompatible information recieved\n");
                exit(-1);
            }
            break;
        }       
      }    
  }



  llclose(fd, RECEIVER);
  return ;
}

