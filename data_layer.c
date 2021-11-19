
#include "data_layer.h"



extern int flag, count;
struct termios oldtio,newtio;




int writeSFrame(int fd, unsigned char C){
    unsigned char set[5];
    int res;

    set[0] = FLAG;
    set[1] = A;
    set[2] = C;
    set[3] = set[1] ^ set[2];
    set[4] = FLAG;

    if (res = write(fd,set,5)<0)
        perror("Error writing\n");  //manda set
    flag = 0;
    sleep(1);
    alarm(TIMEOUT);
    printf("%d bytes written\n", res);
    
    return 0;
  
}

int writeAnswerFrame(int fd, unsigned char C){
  unsigned char set[5];
    int res;

    set[0] = FLAG;
    set[1] = A;
    set[2] = C;
    set[3] = set[1] ^ set[2];
    set[4] = FLAG;

    if (res = write(fd,set,5)<0)
        perror("Error writing\n");  //manda set
    return 0;
}


int recieveSFrame(int fd, unsigned char C){

      unsigned char ua[5];
      int status = 0;
      int res=0;

      int i = 0;
      while (status != 5){

        if (flag) {
          printf("aqui");
          break;
        }
        if (res = read(fd,&ua[i],1)==0){
            continue;
        }
        printf("ua %x \n", ua[i]); 
        i++;
      
        switch (status){
            case 0: 
                if (ua[0]==FLAG){
                    status=1;
                    printf("estado 1\n");
        }
            break;
            case 1:
                if (ua[1] == A){
                    status = 2;
                    printf("estado 2\n");

                }
                else if (ua[1] !=FLAG)
                    status = 0;
                break;

            case 2:
                if (ua[2] == C){
                    status = 3;
                    printf("estado 3\n");
                }
                else if (ua[2] == FLAG)
                    status = 1;
                else 
                    status=0;
                break;

            case 3:
                if (ua[3] == C ^ A){
                    status = 4;
                    printf("estadp 4\n");

                }
                else if (ua[3] == FLAG)
                    status = 1;
                else 
                    status=0;
                break;
            case 4:
                if (ua[4] == FLAG){
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


int llopen(char *port, int flag){
    int fd, notrecieved;

    if (flag != TRANSMITTER && flag != RECEIVER){
        perror("Flag must be 1 or 0."); 
        return -1; 
    }

    if (flag == TRANSMITTER){

        fd = openPort(port);
        (void)signal(SIGALRM,&sig_handler);

        while (count<4 && notrecieved){
          writeSFrame(fd, C_SET);
          if ((notrecieved = recieveSFrame(fd, C_UA))==0){
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
        recieveSFrame(fd, C_SET);
        writeAnswerFrame(fd, C_UA);
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



int llclose(int fd){

 if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }
    
    close(fd);
    return 0;
}


