#include "../lib/app_layer.h"


int createControlPackage(unsigned char flag, char* fileName, int fileSize, unsigned char* package ){
    
    
    package[0] = flag;
    package[1]= FILENAME;
    package[2] = strlen(fileName); 
    printf("PACKAGE %d\n", package[2]);


    if (memcpy(&package[3] , fileName, package[2])==NULL){
        perror("Failed loading file name\n");
        return -1;
    }
    for (int i = 3; i< 3+  strlen(fileName); i++){
        printf("PACKAGE %c\n", package[i]);
    }


    printf("Acabou\n");

    int position = 3 + strlen(fileName);

    char * length_string = (char*)malloc(sizeof(int)); 
    sprintf(length_string, "%d", fileSize);  

    printf("%d", fileSize ); 
    printf("%d", strlen(length_string) ); 


    package[position +1 ] = FILESIZE;
    package[position +2 ] =  strlen(length_string);

    if (memcpy(&package[position + 3] , length_string, strlen(length_string))==NULL){
        perror("Failed loading file name\n");
        return -1;
    }
     for (int i = position +3; i< 3+  strlen(length_string); i++){
        printf("PACKAGE %c\n", package[i]);
    }

  
    return 3 + strlen(fileName) + 2 + strlen(length_string);
}

int createDataPackage(unsigned int seqNum, unsigned int dataSize, unsigned char * data, unsigned char * package){

    printf("DATA PACKAGE CREATEING\n");
    package[0] = DATA;
    package[1] = seqNum % 256;
    package[2] = dataSize / 256;
    package[3] = dataSize % 256;

    if (memcpy(&package[4] , data, dataSize)==NULL){
        perror("Failed loading file name\n");
        return -1;
    }
    printf("DATA SIZE %d", dataSize);
    return 4 + dataSize;

}


int readControlPackage(unsigned char * package, unsigned char * fileName, int* fileSize){
    int fileNameSize;
    unsigned char * fileSize_string = (char*)malloc(sizeof(int));  //ver melhor
    int index = 3; int lengthSize;
    if (package[1] == FILENAME ){
        fileNameSize = package[2];

        for (int i = index; i<fileNameSize;i++){
            if (memcpy(fileName, &package[i], fileNameSize) == NULL){
                perror("Not possible to parse nameFile \n");
                return -1;
        }
    }
    }
    printf("%s\n", fileName);
    index += fileNameSize;
    if (package[index++] == FILESIZE ){
        lengthSize = package[index++];
        for (int i = index; i< index +lengthSize;i++){
            if (memcpy(fileSize, &package[i], lengthSize) == NULL){
                perror("Not possible to parse nameFile \n");
                return -1;
            }
        }
        sscanf(fileSize_string, "%d", fileSize);   
    }
    perror("Read control package."); 
    free(fileSize_string);
    
    return index +lengthSize; 


}


int readDataPackage(unsigned char* package, int* seq,unsigned char * data){

    seq = package[1];
    int size = package[2]* 256 + package[3]%256;

    if (memcpy(data, &package[4], size) == NULL){
        perror("Error parsing info."); 
        return -1; 
    }

    return size; 

}

