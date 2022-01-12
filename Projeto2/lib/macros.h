#ifndef MACROS_H
#define MACROS_H

#define FTP_PORT 21


typedef struct 
{
    char *user;
    char *pass;
    char *host;
    char *path;
} ftp_url;

#endif