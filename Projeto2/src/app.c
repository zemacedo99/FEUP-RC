#include "../lib/app.h"

int main(int argc, char** argv)
{

    
    if (argc != 2 && strcmp(argv[0], "download") != 0)
    {
        printf("Usage: download ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    
    ftp_url args;

    // parse url
    if(parse_url(argv[1], &args) != 0)
    {
        printf("Error parsing the url\n");
        printf("Usage: download ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }
        
    

    // file transfer 
    if(file_transfer(args) != 0)
    {
        printf("Error transfering the file\n");
        return -1;
    }
    

    printf("\nSuccessful file transfer\n");

    return 0;
}