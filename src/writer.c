#include "../lib/writer.h"


void writer(char *port, char * fileName)
{
  int fd;
  unsigned char package[MAX_SIZE];
  unsigned char fileData[MAX_DATA];
  FILE *fp;
  unsigned int seq = 0;
  unsigned int informationMaxSize = MAX_DATA;
  //printf("information size is    %d\n", informationMaxSize);
  unsigned int actualSize = 0;
  unsigned int fileDataSize = 0;

  fd = llopen(port, TRANSMITTER);//open file description and data layer
  /* open file*/
  fp = fopen(fileName, "rb");
  unsigned int fileSize = get_file_size(fp);

  //printf("file size %d \n", fileSize);

  //printf("datas %d\n", fileSize / informationMaxSize);

  //printf("llopen necessita ser executado antes do fopen,fazer um sleep de 1 s ou descomentar este printf\n");
  //sleep(1);


  int size = createControlPackage(START, fileName, fileSize, package); 

  //printf("criei control:) %d\n", size);
 
  llwrite(fd, package, size);  //send control package 

  printf("Sending file...\n");
  while (1){
      if (seq * informationMaxSize > fileSize){
        actualSize = (seq * informationMaxSize) % fileSize;
        //printf("AQUI ENTRI\n");
      }
      else{
        actualSize = informationMaxSize;
        //printf("actual size DATA %d", actualSize);
      }
      if (( fileDataSize = fread(fileData, 1, actualSize, fp)) == 0){
        break;
      }
      for (unsigned int i = 0;i< fileDataSize; i++){
        //printf( " %d estou a enviar %x    ", seq, fileData[i]);
      }
      //printf("SIZEEE %d\n", fileDataSize);

      if ((size = createDataPackage(seq, fileDataSize, fileData, package) )< 0){
        perror("Failed creating data Package\n");
        return;
      }
      //printf("SIZEEE %d\n", size);
      
      if (llwrite(fd, package, size) == -1){
        return;
      }

      seq++;

  }

    
  size = createControlPackage(END, fileName, fileSize, package); 
  llwrite(fd, package, size);



  llclose(fd, TRANSMITTER);

  printf("File sended\n");

 

  return;
}
