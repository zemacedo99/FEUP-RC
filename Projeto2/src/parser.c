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

    // parsing the user, password and host
    if(parse_url_first_part(user_pass_host, args) != 0) 
    {
        return -1;
    }

    // save the path in the struct
    strcpy(args->path, path);

    return 0;

} 



int parse_url_first_part(char* user_pass_host, ftp_url* args){
    
    // storing the first_part to confirm latter
    char first_part[63+63+255];
    strcpy(first_part, user_pass_host);

    char *user = strtok(user_pass_host, ":");

    if(user == NULL) 
    {            
        printf("Wrong URL format\n");
        return -1;
    }

    // no user if the ':' doesn't exists in the initial phrase
    if(strcmp(user, first_part) == 0)
    {         
        // save the user in the struct 
        strcpy(args->user, "anonymous");
        // save the pass in the struct 
        strcpy(args->pass, "");
        // save the host in the struct 
        strcpy(args->host, user);
    } 
    else 
    {                
        // save the user in the struct                   
        strcpy(args->user, user);

        char *pass = strtok(NULL, "@");

        if(pass == NULL)
        {            
            printf("Wrong URL format\n");
            return -1;
        }   

        // save the pass in the struct 
        strcpy(args->pass, pass);
        
        char *host = strtok(NULL, "");
        if(user == NULL)
        {            
            printf("Wrong URL format\n");
            return -1;
        }
        
        // save the host in the struct 
        strcpy(args->host, host);
    }
    return 0;
}
