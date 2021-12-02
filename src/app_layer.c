#include "../lib/app_layer.h"


int createControlPackage(unsigned char flag, char* fileName, int fileSize, unsigned char* package ){
    
    package[0] = flag;
    package[1]= FILENAME;
    package[2] = strlen(fileName) +1 ; 
    //printf("PACKAGE %d\n", package[2]);


    if (memcpy(&package[3] , fileName, package[2])==NULL){
        perror("Failed loading file name\n");
        return -1;
    }
   
    int position = 3 + strlen(fileName);

    char * length_string = (char*)malloc(sizeof(int)); 
    sprintf(length_string, "%d", fileSize);  

    //printf("string %s,int %d \n", length_string); 


    package[position] = FILESIZE;
    package[position +1 ] =  strlen(length_string) ;

    if (memcpy(&package[position + 2] , length_string, strlen(length_string))==NULL){
        perror("Failed loading file name\n");
        return -1;
    }

    /*
    for (int i = position +2; i< position +2+  strlen(length_string); i++){
        printf("PACKAGE %c\n", package[i]);
    }
    */

  
    return 3 + strlen(fileName) + 2 + strlen(length_string);
}

int createDataPackage(unsigned int seqNum, unsigned int dataSize, unsigned char * data, unsigned char * package){

    //printf("DATA PACKAGE CREATEING\n");
    package[0] = DATA;
    package[1] = seqNum % 256;
    package[2] = dataSize  / 256;
    package[3] = dataSize  % 256;

    if (memcpy(&package[4] , data, dataSize) ==NULL){
        perror("Failed loading file name\n");
        return -1;
    }

    //printf("DATA SIZE %d", dataSize);
    return 4 + dataSize;

}


int readControlPackage(unsigned char * package,  char * fileName, int* fileSize, int size){
    int fileNameSize;
    char fileSize_string[256];  //ver melhor
    int index = 3; int lengthSize;
    fileName[0] = 'c';
    if (package[1] == FILENAME ){
        fileNameSize = package[2];


        if (memcpy(&fileName[1], &package[3], fileNameSize) == NULL){
            perror("Not possible to parse nameFile \n");
            return -1;
        }

        /*
        for (int i= 0; i< fileNameSize +1; i++){
            printf("recebi file %c\n", fileName[i]);
        }
        */
    
    
    }
    //printf("readed file name size : %d \n", fileNameSize);
    //printf("readed file name: %s\n", fileName);
    index += fileNameSize -1;
    if (package[index++] == FILESIZE ){

        //printf("OKAY ENTREI\n");
        lengthSize = package[index++];
        //printf("string length size %d\n", lengthSize);
        if (memcpy(fileSize_string, &package[index], lengthSize) == NULL){
            perror("Not possible to parse nameFile \n");
            return -1;
        }
       
        //printf("string %s\n", fileSize_string);

    
        *fileSize = atoi(fileSize_string); 

        //printf("tamanho %d\n", *fileSize);
    }
    
    return index +lengthSize; 

}


int readDataPackage(unsigned char* package, int* seq,unsigned char * data){

    *seq = package[1];
    int size = package[2]* 256 + package[3] ;

    //printf("read package size %d\n", size);

    if (memcpy(data, &package[4], size) == NULL){
        perror("Error parsing info."); 
        return -1; 
    }

    return size; 

}

