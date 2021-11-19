#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "alarme.h"
 int flag=0, count=0, i = 0;





void sig_handler(int signum){
   printf("ALARME # %d\n", i);
   i++;
   flag=1;
   printf("flag %d\n", flag);
	count++;

}

