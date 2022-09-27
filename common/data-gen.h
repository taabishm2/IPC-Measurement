#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

enum
{
    RETURN_ACK = 0, // Receiver sends back a tiny ACK
    RETURN_MIRROR   // Receiver sends back entire msg 
};

char* getRandomString(size_t);

char* getRandomString(size_t length) {
    char *str = malloc(length + 1);
    memset(str, 'a', length);
    str[length] = '\0';
    return str;
}
