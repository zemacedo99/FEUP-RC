# FEUP-RC

## Download application

Simple FTP application that retrieves a specified file using the FTP protocol as described in RFC959.

It should take an argument that adopts the URL syntax, as described in RFC1738. 


Example:

``download ftp://ftp.up.pt/pub/kodi/timestamp.txt``

Where the URL path is

``ftp://[<user>:<password>@]<host>/<url-path>``

The files:

    clientTCP -> establish a TCP connection (using SOCKETs) to a server and send a string
    getip -> receives as a parameter a host name and returns its IP address