#include "../lib/alarme.h"

int alarmActive = FALSE, count=0, i = 0;





void sig_handler(int signum){
   printf("ALARME # %d\n", i);
   i++;
   alarmActive = TRUE;
   printf("flag %d\n", alarmActive);
	count++;

}

