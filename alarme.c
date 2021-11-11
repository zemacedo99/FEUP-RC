#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "alarme.h"
 int flag=0, conta=0, i = 1;





void sig_handler(int signum){
   printf("alarme # %d\n", i);
   i++;
   flag=1;
	conta++;

}

