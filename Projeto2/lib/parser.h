#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

int parse_url(const char* url, ftp_url* args);
int parse_url_first_part(char* user_pass_host, ftp_url* args);
int parse_server_response(const char* response_string, ftp_server_response* response, const char* delimiter);
int parse_filename(char* path, char* filename);

#endif