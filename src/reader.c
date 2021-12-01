#include "../lib/reader.h"

void reader(char *port)
{
  int fd;int res;
  int seq = 0;
  char * fileName = (char*)malloc(sizeof(char)*MAX_DATA_SIZE);
  FILE *fp;
  int length; int fileSize;
  unsigned char package[MAX_DATA_SIZE];
  unsigned char data[MAX_DATA_SIZE];


  
  fd = llopen(port, RECEIVER );

  printf("AQUI LOGO DEPOIS DO LLOPEN\n");


  //recieve control package

  while (TRUE){
    res = llread(fd, package);
    printf("sai do ll read\n");
    if (package[0]==START){
      if (readControlPackage(package, fileName, &fileSize)>0)
          break;
    }
  }

  printf("LI CONTROL PACKAGE FIXE\n");

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
        fwrite(data, 1, length, fp); 
        printf("AQUI LOGO DEPOIS DO READ PACKAGE %d\n", seq);


      }

      else if (package[0] == END){
        char * endFileName = (char*)malloc(sizeof(char)*MAX_DATA_SIZE);
        int endSize;

        if (readControlPackage(package, endFileName, &endSize)>0){
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

