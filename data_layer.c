
#include "data_layer.h"



extern int alarmActive, count;
struct termios oldtio,newtio;
int currentNs = 0, currentNr = 1;       // Ns sent is by emitter , Nr is sent by receiver

int writeIFrame(int fd, unsigned char *msg,  int lenght){
    unsigned char frame[FRAME_SIZE];
    int res;

    if(lenght > MAX_DATA_D){
        printf("Error: Data size to big to send\n");
        return -1;
    }
    
    frame[0] = FLAG;
    frame[1] = A_E;
    frame[2] = C_I(currentNs);
    frame[3] = frame[1]  ^ frame[2];
    
    char currentXOR = frame[3];

    int i, j;
 
    for(i = 4, j = 0; j < lenght; i++, j++){      //i iterates over frame array. j iterates over msg array
        
        //Byte Stuffing
        if(msg[j] == FLAG){                     
            
            frame[i++] = ESC;
            frame[i] = FLAG_ESC;
        }else if(msg[j] == ESC){
            
            frame[i++] = ESC;
            frame[i] = ESC_ESC;
        }else {

            frame[i] = msg[j];            
        }
        currentXOR = currentXOR ^ msg[j];

    }

    //BCC2
    if(currentXOR == FLAG){                     
        
        frame[i++] = ESC;
        frame[i++] = FLAG_ESC;
    }else if(currentXOR == ESC){
        
        frame[i++] = ESC;
        frame[i++] = ESC_ESC;
    }else {

        frame[i++] = currentXOR;            
    }  

    frame[i++] = FLAG;

    res = write(fd, frame, i); 
    sleep(1);
  
    

    return res;
}


int writeFrame(int fd, unsigned char A, unsigned char C){
    unsigned char frame[5];
    int res;

    frame[0] = FLAG;
    frame[1] = A;
    frame[2] = C;
    frame[3] = A ^ C;
    frame[4] = FLAG;

    res = write(fd,frame,5);
    for (int i= 0 ; i<5; i++){
        printf("enviei %x\n", frame[i]);
    }

    if (res<0){
        perror("Error writing\n");  //manda set
        return -1;

    }

    printf("%d bytes written alarme active  %d\n", res, alarmActive);
    sleep(1);
  

    return 0;
  
}


int recieveSFrame(int fd,unsigned char A, unsigned char C){

      unsigned char frame[5];
      int status = 0;
      int res;

      int index = 0;
      while (status != 5){

        if (alarmActive) {
          printf("aqui");
          break;
        }

    
        if (index > 4) index = 0;

        res = read(fd,&frame[index],1);
        if (res==0){
            continue;
        }
        printf("recebi %d byte %x \n",res, frame[index]); 

        index++;

        switch (status){
            case 0: 
                if (frame[0]==FLAG){
                    status=1;
                    printf("estado 1\n");
            }
            break;
            case 1:
                if (frame[1] == A){
                    status = 2;
                    printf("estado 2\n");

                }
                else if (frame[1] !=FLAG){
                    status = 0;
                    printf("help nao sei ler %x\n", frame[1]);

                }
            break;

            case 2:
                if (frame[2] == C){
                    status = 3;
                    printf("estado 3\n");
                }
                else if (frame[2] == FLAG)
                    status = 1;
                else 
                    status=0;
            break;

            case 3:
                if (frame[3] == C ^ A){
                    status = 4;
                    printf("estadp 4\n");

                }
                else if (frame[3] == FLAG)
                    status = 1;
                else 
                    status=0;
            break;
            case 4:
                if (frame[4] == FLAG){
                  printf("status 5\n");
                  status = 5;
                }
                else 
                    status=0;
            break;
            }
        }
        if (status != 5){
            return 1;
        }

    return 0;

}


int llopen(char *port, unsigned char flag){
    int fd, notrecieved;

    if (flag != TRANSMITTER && flag != RECEIVER){
        perror("Flag must be 1 or 0."); 
        return -1; 
    }

    if (flag == TRANSMITTER){

        fd = openPort(port);

        while (count<4 && notrecieved){
            writeFrame(fd, A_E, C_SET);
            activateAlarm();  
            if ((notrecieved = recieveSFrame(fd,A_E, C_UA))==0){
                desactivateAlarm();
                break;

            }            
        }
        if (notrecieved){
          perror("TIMEOUT");
          exit(-1);
        }
    }
    else if (flag == RECEIVER){
        fd = openPort(port);
        recieveSFrame(fd ,A_E ,C_SET);
        printf("recebi a frame \n");
        sleep(2);
        writeFrame(fd, A_E, C_UA);
    }

    return fd;

}



int openPort(char *port){

    int fd;
       
    fd = open(port, O_RDWR | O_NOCTTY);
    if (fd <0) {perror(port); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      return(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */

    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      return(-1);
    }

    printf("New termios structure set\n");
    
    return fd;

}
 
 void activateAlarm(){

    (void)signal(SIGALRM,&sig_handler);
     printf("ACTIVATE ALARM\n");
    alarmActive = FALSE;
    alarm(TIMEOUT);

 }

void desactivateAlarm(){
    printf("DESACTIVATE ALARM\n");
    alarm(0);
    count = 0;
    alarmActive = FALSE;
}

int llclose(int fd, unsigned char flag ){
    count = 0;
    int notrecieved = 1;
    
    if (flag ==TRANSMITTER){
        while (count<4 && notrecieved){
            sleep(4);
            writeFrame(fd, A_E, C_DISC);
            printf("Enviei o DISC\n");
            activateAlarm();
            if ((notrecieved = recieveSFrame(fd, A_R, C_DISC))==0){
                printf("recebi o disc\n");
                desactivateAlarm();
                break;
            }
        }
        if(notrecieved){
            perror("Not recieved DISC frame\n");
            exit(-1);
        }
        else{
            writeFrame(fd, A_R, C_UA);
            printf("enviei a resposta ao disc\n");
        }

    }
    else if (flag == RECEIVER){
        while (count<4 && notrecieved){
            activateAlarm();
            if ((notrecieved = recieveSFrame(fd, A_E, C_DISC))==0){
                desactivateAlarm();
                printf("recebi  disc RECETOR\n");
                break;
            }
        }
        if(notrecieved){
            perror("TIMEOUT: Not recieved DISC frame\n");
            exit(-1);
        }
        else{
            count = 0;
            notrecieved = 1;
            while (count<4 && notrecieved){

                writeFrame(fd, A_R, C_DISC);
                printf("escrevi o disc RECETOR\n");
                activateAlarm();
                if ((notrecieved = recieveSFrame(fd, A_R, C_UA))==0){
                    printf("recebi resposta ao disc RECETOR\n");
                    desactivateAlarm();
                    break;
                }
            }
            if(notrecieved){
                perror("TIMEOUT: Not recieved answer to DISC frame frame\n");
                exit(-1);
            }
            

        }
        
        
    }


    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
        perror("tcsetattr");
        exit(-1);
        }
        
        close(fd);
        return 0;
}


