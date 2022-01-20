#ifndef MACROS_H
#define MACROS_H

#define FTP_PORT 21

#define USER_LEN 63
#define PASS_LEN 63
#define HOST_LEN 255
#define PATH_LEN 255

#define DESC_LEN 127

#define SERVER_RESPONSE_LEN 255

#define READY_FOR_NEW_USER 220 
#define USER_LOGIN_OK 230
#define NEED_PASSWORD 331
#define ENTER_PASV_MODE 227


#define MAX_TRIES 3



typedef struct 
{
    char user[USER_LEN];
    char pass[PASS_LEN];
    char host[HOST_LEN];
    char path[PATH_LEN];
} ftp_url;


typedef struct 
{
    int code;
    char description[DESC_LEN];
} ftp_server_response;

typedef struct 
{
    int h1;
    int h2;
    int h3;
    int h4;
} IPv4_address;



#endif