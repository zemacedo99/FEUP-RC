


#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define FLAG 0x7e
#define A_E 0x03
#define A_R 0x01
#define C_SET 0x03
#define TIMEOUT 3
#define C_UA 0x07
#define C_DISC 0x0B
#define C_I(x) (unsigned char )(x << 6)

#define FRAME_SIZE 16384  // Maximum size of frame of data information
#define MAX_DATA_D (FRAME_SIZE - 6) / 2 // Maximum size of data that can be send


#define MAX_SIZE 1600

#define START 2
#define END 3
#define FILENAME 1
#define FILESIZE 0
//Byte Stuffing
#define ESC 0x7d
#define FLAG_ESC 0x5e
#define ESC_ESC 0x5d


#define TRANSMITTER 0x01
#define RECEIVER 0X00