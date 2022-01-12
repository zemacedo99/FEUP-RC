#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

int parse_url(const char* url, ftp_url* args);

#endif