#include "../lib/parser.h"

int parse_url(const char* url, ftp_url* args){

    char* app_protocol = strtok(url, "//");

    // check if it is an FTP Application Protocol
    if( strcmp(app_protocol, "ftp:") || app_protocol == NULL)
    {           
        printf("Application protocol not accepted, pls choose ftp protocol \n");
        return -1;
    }

    // url first part (user, pass and host)
    char* user_pass_host = strtok(NULL, "/");
    if(user_pass_host == NULL){            
        printf("URL wrong format \n");
        return -1;
    }

    // url second part (path)
    char* path = strtok(NULL, " ");
    if(path == NULL){           
        printf("URL wrong format \n");
        return -1;
    }
    // save the path in the struct
    strcpy(args->path, path);

    args->host = "ftp.up.pt";
    // TODO: parse the user, password and host

    return 0;

} 