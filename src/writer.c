#include "../lib/writer.h"


void writer(char *port, char * fileName)
{
  int fd;
  unsigned char package[MAX_SIZE];
  unsigned char fileData[MAX_DATA];
  FILE *fp;
  unsigned int seq = 0;
  unsigned int informationMaxSize = MAX_DATA - 8;
  unsigned int actualSize = 0;
  unsigned int fileDataSize = 0;

/* open file*/
  fp = fopen(fileName, "rb");
  unsigned int fileSize = get_file_size(fp);

  printf("file size %d \n", fileSize);
  printf("datas %d\n", fileSize / informationMaxSize);

  printf("llopen\n");

  fd = llopen(port, TRANSMITTER);//open file description and data layer


  int size = createControlPackage(START, fileName, fileSize, package); 

  printf("criei control:) %d\n", size);
 
  llwrite(fd, package, size);  //send control package 


  while (1){
      if (seq * informationMaxSize > fileSize){
        actualSize = (seq * informationMaxSize) % fileSize;
        printf("AQUI ENTRI\n");
      }
      else{
        actualSize = informationMaxSize;
        printf("actual size DATA %d", actualSize);
      }
      if (( fileDataSize = fread(fileData, 1, actualSize, fp)) == 0){
        break;
      }
      printf("SIZEEE %d\n", fileDataSize);

      if ((size = createDataPackage(seq, fileDataSize, fileData, package) )< 0){
        perror("Failed creating data Package\n");
        return -1;
      }
      printf("SIZEEE %d\n", size);
      for (int i = 0;i< size; i++){
        printf( " %d estou a enviar %x    ", seq, package[i]);
      }
      if (llwrite(fd, package, size) == -1){
        return (-1);
      }

      seq++;

  }

    
  size = createControlPackage(END, fileName, fileSize, package); 
  llwrite(fd, package, size);



  llclose(fd, TRANSMITTER);


 

  return;
}
