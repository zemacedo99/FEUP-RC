#ifndef MACROS_H
#define MACROS_H

#define USER_LEN 63
#define PASS_LEN 63
#define HOST_LEN 255
#define PATH_LEN 255

#define DESC_LEN 127

#define SERVER_RESPONSE_LEN 255

#define FTP_PORT 21


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


#endif