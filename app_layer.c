#include "app_layer.h"


int sendPackage(){
    return 0;
}

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

    int position = 3 + package[2];

    char * length_string = (char*)malloc(sizeof(int)); 
    sprintf(length_string, "%d", fileSize);   

    package[position +1 ] = FILESIZE;
    package[position +2 ] =  strlen(length_string);

    if (memcpy(&package[position + 3] , length_string, strlen(length_string))==NULL){
        perror("Failed loading file name\n");
        return -1;
    }
    for (int i = position +3; i < position + 3 + strlen(fileName); i++){
        printf("PACKAGE %c\n", package[i]);
    }

    for (int i = 0; i<position + 3  +  strlen(length_string); i++){
        printf("PACKAGE %x\n", package[i]);
    }

    return 0;
}

int createDataPackage(unsigned int seqNum, unsigned int dataSize, unsigned char * data, unsigned char * package){

    package[0] = DATA;
    package[1] = seqNum % 256;
    package[2] = dataSize / 256;
    package[3] = dataSize % 256;

    if (memcpy(&package[4] , data, dataSize)==NULL){
        perror("Failed loading file name\n");
        return -1;
    }

    return 0;

}

