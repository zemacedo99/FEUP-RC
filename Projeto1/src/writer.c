#include "../lib/writer.h"


void writer(char *port, char * fileName)
{
  int fd;
  unsigned char package[MAX_SIZE];
  unsigned char fileData[MAX_DATA];
  FILE *fp;
  unsigned int seq = 0;
  unsigned int informationMaxSize = MAX_DATA;
  unsigned int actualSize = 0;
  unsigned int fileDataSize = 0;

  fd = llopen(port, TRANSMITTER);//open file description and data layer
  /* open file*/
  fp = fopen(fileName, "rb");
  unsigned int fileSize = get_file_size(fp);


  int size = createControlPackage(START, fileName, fileSize, package); 

 
  llwrite(fd, package, size);  //send control package 

  printf("Sending file...\n");
  while (1){
      if (seq * informationMaxSize > fileSize){ //checks if it's not the end of the file
        actualSize = (seq * informationMaxSize) % fileSize;
      }
      else{
        actualSize = informationMaxSize;
      }
      if (( fileDataSize = fread(fileData, 1, actualSize, fp)) == 0){
        break;
      }
  

      if ((size = createDataPackage(seq, fileDataSize, fileData, package) )< 0){ // stops sending data packages if there is no more data to be sent
        perror("Failed creating data Package\n");
        return;
      }
      
      if (llwrite(fd, package, size) == -1){
        return;
      }

      seq++;

  }

    
  size = createControlPackage(END, fileName, fileSize, package);  //send final control package
  llwrite(fd, package, size);



  llclose(fd, TRANSMITTER);

  printf("File sent\n");

 

  return;
}
