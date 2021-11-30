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


  //recieve control package

  while (TRUE){
    res = llread(fd, package);
    if (package[0]==START){
      if (readControlPackage(package, fileName, &fileSize)==0)
          break;
    }
  }
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
        res = readDataPackage(package, &seq, data);//POR FAZER
        fwrite(data, 1, length, fp); 

      }

      else if (package[0] == END){
        if (readControlPackage(package, fileName, &fileSize)==0){
          //CHECAR SE OS VALORES RETORNADOS ESTÃO BEM

        }

      }
      

  }



  llclose(fd, RECEIVER);
  return ;
}

