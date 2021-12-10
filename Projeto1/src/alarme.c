#include "../lib/alarme.h"

int alarmActive = FALSE, count=0, i = 0;





void sig_handler(){

    i++;
    alarmActive = TRUE;
    count++;


}

