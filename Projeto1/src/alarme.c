#include "../lib/alarme.h"

int alarmActive = FALSE, count=0;




void sig_handler(){
    printf("ALARM #%d\n", count);

    alarmActive = TRUE;
    count++;


}

