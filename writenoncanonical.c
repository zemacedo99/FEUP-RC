/*Non-Canonical Input Processing*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "alarme.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define FLAG 0x7e
#define A 0x03
#define SET 0x03

extern int flag, conta;

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];
    unsigned char set[5];
   
    
   
    int i, sum = 0, speed = 0;
    
    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }

    (void)signal(SIGALRM,&sig_handler);


    set[0] = FLAG;
    set[1] = A;
    set[2] = SET;
    set[3] = set[1] ^ set[2];
    set[4] = FLAG;

   /* Open serial port device for reading and writing and not as controlling tty
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
    leitura do(s) pr髕imo(s) caracter(es)
  */

    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");

  /* 
    O ciclo FOR e as instru珲es seguintes devem ser alterados de modo a respeitar 
    o indicado no gui鉶 
  */

    int status = 0;




    while(conta < 4){
      //write 
      //alarme
      //le e checa maquina de estados 
      

      res = write(fd,set,5);  //manda set
      flag = 0;
      alarm(2);
      printf("%d bytes written\n", res);
  


      sleep(2);
      char bufread[255];
      unsigned char ua[5];

      i = 0;
      while (status != 5){
        if (flag) break;
        res = read(fd,&ua[i],1);
        printf("ua %x \n", ua[i]); 
        i++;
        switch (status){
            case 0: 
                if (ua[0]==0x7e)
                    status=1;
            case 1:
                if (ua[1] == 0x01)
                    status = 2;
                else if (ua[1] != 0x7e)
                    status = 0;
            case 2:
                if (ua[2] == 0x07)
                    status = 3;
                else if (ua[2] == 0x7e)
                    status = 1;
                else 
                    status=0;
            case 3:
                if (ua[3] == ua[2] ^ ua[1])
                    status = 4;
                else if (ua[3] == 0x7e)
                    status = 1;
                else 
                    status=0;
            case 4:
                if (ua[4] == 0x7e)
                    status = 5;
                else 
                    status=0;
            }
        }

      }

   
    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }
    
    if (status !=5 ){
      perror("TIMEOUT");
      exit(-1);
    }
    




    close(fd);
    return 0;
}
