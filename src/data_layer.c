 #include "../lib/data_layer.h"



extern int alarmActive, count;
struct termios oldtio,newtio;
int NS = 0, NR = 1;       // NS is send by emitter , NR is sent by receiver
int expectedNS = 0, expectedNR = 1;  // NS expected by the receiver and NR expected by the emitter

int writeIFrame(int fd, unsigned char *msg,  int lenght){
    unsigned char frame[FRAME_SIZE];
    int res;

    //printf("lenght da data%d\n", lenght);
    //printf("lenght da data%d\n", MAX_PACKAGE_SIZE);


    if(lenght > MAX_PACKAGE_SIZE){
        printf("Error: Data size to big to send\n");
        return -1;
    }
    
    frame[0] = FLAG;
    frame[1] = A_E;
    frame[2] = C_I(NS);
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
    //sleep(1);
  
    

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
    

    if (res<0){
        perror("Error writing\n");  //manda set
        return -1;

    }

    //sleep(1);
  

    return 0;
  
}



int receiveIFrame(int fd, unsigned char *buffer)
{

    unsigned char frame;
    unsigned char bcc1;
    int status = 0;
    int res;

    while (status != 4){

        if (alarmActive) {
            //printf("alarm is active \n");
            break;
        }



        res = read(fd,&frame,1);
        if (res==0){
            continue;
        }

        //printf("recebi %d byte %x \n",res, frame); 


        switch (status){
            case 0: 
                if (frame==FLAG){
                    status=1;
                }
            break;
            case 1:
                if (frame == A_E){
                    status = 2;
                    //printf("receiveIFrame: status 2\n");

                }
                else if (frame !=FLAG){
                    status = 0;
                    //printf("receiveIFrame: help nao sei ler %x\n", frame);

                }
            break;

            case 2:
                if (frame == C_I(expectedNS)){
                    status = 3;
                    //printf("receiveIFrame: status 3\n");
                }
                else if (frame == C_I(1) || frame== C_I(0)){
                    return -2; //Tramas I duplicadas
                }
                else if (frame == FLAG)
                    status = 1;
                else 
                    status=0;
            break;

            case 3:
                if (frame == (C_I(expectedNS) ^ A_E)){
                    status = 4;
                    //printf("receiveIFrame: status 4\n"); 
                    bcc1 = frame;
                    res = receiveIData(fd, bcc1, buffer);
                    
                    return res;

                }
                else if (frame == FLAG)
                    status = 1;
                else 
                    status=0;
            break;
            default:
            break;
        
        }
    }

    return REJ;  // Trama I sem erros
}

int receiveIData(int fd, unsigned char  bcc1, unsigned char  *buffer){

    int bufferSize = 0;
    int res;
    unsigned char frame = 0;

    unsigned char currentXOR = bcc1;

    
    while(1){
        
        res = read(fd,&frame,1);
        if (res==0){
            continue;
        }

        if(frame == FLAG)
        {
            //printf("destuffing done\n");
            break;
        }
        //Byte destuffing

        if(frame == ESC){

            res = read(fd,&frame,1);
            if (res==0){
                continue;
            }

            if(frame == FLAG_ESC){
                buffer[bufferSize++] = FLAG;
            }else if(frame == ESC_ESC){
                buffer[bufferSize++] = ESC;
            }

        }else{
            
            buffer[bufferSize++] = frame;
        }        
        
    }


    for (int i = 0; i <= bufferSize -2;i++){

        currentXOR = currentXOR ^ buffer[i];
    }



    if(currentXOR != buffer[bufferSize - 1]){
        printf("Frame with error, bcc2 does not match");
        return -1; //Trama I com erros
    }

    //printf("vou sair do destuffing\n");

    return bufferSize - 1;
}

int receiveRFrame(int fd)
{

    unsigned char frame;
      int status = 0;
      int res;
      unsigned char C = 0;
      int answerType;
    
      while (status != 5){

        if (alarmActive) {
          return ANSWER_TIMEOUT;
        }
    
        
        if ((res = read(fd,&frame,1))==0)   
            continue;

        //printf("recebi %d byte %x \n",res, frame); 


        switch (status){
            case 0: 
                if (frame==FLAG){
                    status=1;
                    //printf("receiveRFrame: status 1\n");
            }
            break;
            case 1:
                if (frame == A_E){
                    status = 2;
                    //printf("receiveRFrame: status 2\n");

                }
                else if (frame !=FLAG){
                    status = 0;
                    //printf("help nao sei ler %x\n", frame);
                }
            break;

            case 2:
                if (frame == C_RR(expectedNR)){
                    C = frame;
                    status = 3;
                    answerType = RR;
                    //printf("receiveRFrame: status 3\n");
                }
                else if (frame == C_RR(0) || frame == C_RR(1) ){
                    C = frame;
                    answerType = RR_REPEAT; // wrong C answear repeated
                }
                else if ( frame == C_REJ(expectedNR)){
                    C = frame;
                    answerType =REJ; 
                    
                }
                else if ( frame == C_REJ(0)|| frame == C_REJ(1)){
                    C = frame;
                    answerType =RR_REPEAT; 
                    
                }
                else if (frame == FLAG)
                    status = 1;
                else 
                    status=0;
            break;

            case 3:
                if (frame == (C ^ A_E)){
                    status = 4;
                    //printf("receiveRFrame: status 4\n");

                }
                else if (frame== FLAG)
                    status = 1;
                else 
                    status=0;
            break;
            case 4:
                if (frame == FLAG){
                  //printf("receiveRFrame: status 5\n");
                  status = 5;
                }
                else 
                    status=0;
            break;

            default:
            break;
            }
        }
        

    //printf("returning here r %d\n", answerType);
    return answerType;


}

int recieveSFrame(int fd,unsigned char A, unsigned char C){

      unsigned char frame;
      int status = 0;
      int res;

      while (status != 5){

        if (alarmActive) {
          return ANSWER_TIMEOUT;
        }
    
        
        if ((res = read(fd,&frame,1))==0)   
            continue;

        //printf("recebi %d byte %x \n",res, frame); 


        switch (status){
            case 0: 
                if (frame==FLAG){
                    status=1;
                    //printf("receiveSFrame: status 1\n");
            }
            break;
            case 1:
                if (frame == A){
                    status = 2;
                    //printf("receiveSFrame: status 2\n");

                }
                else if (frame !=FLAG){
                    status = 0;
                    //printf("help nao sei ler %x\n", frame);
                }
            break;

            case 2:
                if (frame == C){
                    status = 3;
                    //printf("receiveSFrame: status 3\n");
                }
                else if (frame == FLAG)
                    status = 1;
                else 
                    status=0;
            break;

            case 3:
                if (frame == (C ^ A)){
                    status = 4;
                    //printf("estreceiveSFrame: statusadp 4\n");

                }
                else if (frame== FLAG)
                    status = 1;
                else 
                    status=0;
            break;
            case 4:
                if (frame == FLAG){
                  //printf("receiveSFrame: status 5\n");
                  status = 5;
                }
                else 
                    status=0;
            break;

            default:
			    break;


            }
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
        //printf("recebi a Trama de Supervisão \n");
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
    //printf("ACTIVATE ALARM\n");
    alarmActive = FALSE;
    alarm(3);

}

void desactivateAlarm(){
    //printf("DESACTIVATE ALARM\n");
    alarm(0);
    count = 0;
    alarmActive = FALSE;
}

int llclose(int fd, unsigned char flag ){
    count = 0;
    int notrecieved = 1;
    
    if (flag ==TRANSMITTER){
        while (count<4 && notrecieved){
            //sleep(4);
            writeFrame(fd, A_E, C_DISC);
            //printf("Enviei o DISC\n");
            activateAlarm();
            if ((notrecieved = recieveSFrame(fd, A_R, C_DISC))==0){
                //printf("recebi o DISC\n");
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
            //printf("enviei a resposta ao disc\n");
        }

    }
    else if (flag == RECEIVER){
        while (count<4 && notrecieved){
            activateAlarm();
            if ((notrecieved = recieveSFrame(fd, A_E, C_DISC))==0){
                desactivateAlarm();
                //printf("recebi  disc RECETOR\n");
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
                //printf("escrevi o disc RECETOR\n");
                activateAlarm();
                if ((notrecieved = recieveSFrame(fd, A_R, C_UA))==0){
                    //printf("recebi resposta ao disc RECETOR\n");
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


int llwrite(int fd, unsigned char * buffer, int length)
{
    //printf("here\n");
    count = 0;
    int numberWrittenChars, r;
    
    while(count<4)
    {
        numberWrittenChars = writeIFrame(fd, buffer, length);
        activateAlarm();
        //printf("ALARME NUMERO %d\n", count); 

        r = receiveRFrame( fd);
        //printf("%d\n", r);

        if(r == RR || r == RR_REPEAT)
        {
            desactivateAlarm();
            printf("Answer RR recieved\n");
            break;
        }
        else if (r == REJ){
            desactivateAlarm();
            printf("Answer REJ recieved\n");

        }
        
    }
    if( count == 4){
        perror("TIMEOUT\n");
        return -1;
    }
    updateSenderN();

    return numberWrittenChars;
}

int llread(int fd, unsigned char * buffer)
{
    while(1)
    {
        int r = receiveIFrame(fd, buffer);
        //printf("recebi e processei a i frame o que é que vou enviar? %d\n", r);
        

        if(r == -1)
        {
            writeFrame(fd, A_E, C_REJ(NR));
        }
        else if(r == -2){
            writeFrame(fd, A_E, C_RR(NR));
            //printf("sair \n");

        }
        else if (r >= 0)
        {
            writeFrame(fd, A_E, C_RR(NR));
            updateRecieverN();
            //printf("sair \n");
            return r;
        }
        
    }
}

void updateRecieverN(){
    if (expectedNS == 1) expectedNS = 0;
    else expectedNS = 1;

    if (NR == 1) NR = 0;
    else NR = 1;
}

void updateSenderN(){
    if (expectedNR == 1) expectedNR = 0;
    else expectedNR = 1;

    if (NS == 1) NS = 0;
    else NS = 1;
}

