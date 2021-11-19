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
#include "macros.h"
#include "data_layer.h"



extern int flag, conta;

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    
    int fd,c, res;
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


    fd = llopen(argv[1], TRANSMITTER);

    int status = 0;
  

    while(conta < 4 && status!=5){
      //write 
      //alarme
      //le e checa maquina de estados 

      printf("vou fazer write\n");
      

      res = write(fd,set,5);  //manda set
      flag = 0;
      sleep(1);
      alarm(2);
      printf("%d bytes written\n", res);
  


      char bufread[255];
      unsigned char ua[5];

      i = 0;
      while (status != 5){

        if (flag==1) {
          printf("aqui");
          break;
        }
        res = read(fd,&ua[i],1);
        if (res==0){
            continue;
        }
        printf("ua %x \n", ua[i]); 
        i++;
      
        switch (status){
            case 0: 
                if (ua[0]==0x7e){
                    status=1;
                    printf("estado 1\n");
        }
            break;
            case 1:
                if (ua[1] == 0x01){
                    status = 2;
                    printf("estado 2\n");

                }
                else if (ua[1] != 0x7e)
                    status = 0;
                break;

            case 2:
                if (ua[2] == 0x07){
                    status = 3;
                    printf("estado 3\n");
                }
                else if (ua[2] == 0x7e)
                    status = 1;
                else 
                    status=0;
                break;

            case 3:
                if (ua[3] == ua[2] ^ ua[1]){
                    status = 4;
                    printf("estadp 4\n");

                }
                else if (ua[3] == 0x7e)
                    status = 1;
                else 
                    status=0;
                break;
            case 4:
                if (ua[4] == 0x7e){
                  printf("status 5\n");
                  status = 5;
                }
                else 
                    status=0;
                break;
            }
        }

      }

    
    if (status !=5 ){
      perror("TIMEOUT");
      exit(-1);
    }
    
    llclose(fd);

    return 0;
}
