#ifndef MACROS_H
#define MACROS_H

#define USER_LEN 63
#define PASS_LEN 63
#define HOST_LEN 255
#define PATH_LEN 255

#define FTP_PORT 21


typedef struct 
{
    char user[USER_LEN];
    char pass[PASS_LEN];
    char host[HOST_LEN];
    char path[PATH_LEN];
} ftp_url;

#endif