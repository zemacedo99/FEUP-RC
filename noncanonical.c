/*Non-Canonical Input Processing*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;
unsigned char set[5];
unsigned char ua[5];

int main(int argc, char** argv)
{
    int fd,c, res;
    int status = 0;
    struct termios oldtio,newtio;


    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
  
    
    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */

    

  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) próximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");
    int i = 0;
  

   

    while (status != 5){
        res = read(fd,&set[i],1);
        printf("%x\n", set[i]); 
        i++;
        switch (status){
            case 0: 
                if (set[0]==0x7e)
                    status=1;
            case 1:
                if (set[1] == 0x03)
                    status = 2;
                else if (set[1] != 0x7e)
                    status = 0;
            case 2:
                if (set[2] == 0x03)
                    status = 3;
                else if (set[2] == 0x7e)
                    status = 1;
                else 
                    status=0;
            case 3:
                if (set[3] == set[2] ^ set[1])
                    status = 4;
                else if (set[3] == 0x7e)
                    status = 1;
                else 
                    status=0;
            case 4:
                if (set[4] == 0x7e)
                    status = 5;
                else 
                    status=0;
        }
}

        ua[0]=0x7e;
        ua[1]= 0x01;
        ua[2] = 0x01;//0x07;
        ua[3] = ua[1] ^ ua[2];
        ua[4] = 0x7e;
            


  /* 
    O ciclo WHILE deve ser alterado de modo a respeitar o indicado no guião 
  */

    /*
        Devolução da palavra
    */

    res = write(fd,ua,5); 
    printf("%d", res);  
    for(int i = 0; i<res; i++){
       write(fd,ua[i],1);  
       printf("%x\n", ua[i]); 
     
    }
 

    sleep(2);
    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}
