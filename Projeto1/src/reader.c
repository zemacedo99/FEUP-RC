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



  //recieve control package

  while (TRUE){
    res = llread(fd, package);
    if (package[0]==START){
      if (readControlPackage(package, fileName, &fileSize)>0)
          break;
    }
  }

  

  if( (fp = fopen(fileName, "wb")) == NULL ) {
        perror("Failed creating file\n");
        exit(-1);
    } 
    

  printf("Receiving file...\n");
  while (TRUE){
      if ( (length = llread(fd,package)) < 0){
            perror("Could not read file descriptor.");
            exit(-1);
        }
      

      if (package[0]== DATA){
        if ((res = readDataPackage(package, &seq, data))<0){
          exit (-1);
        };

  
        fwrite(data, 1, res, fp); 


      }

      else if (package[0] == END){
        char endFileName[256];
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

  printf("File received\n");
  return ;
}

