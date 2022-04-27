#include <stdio.h>

#include "ip.h"
#include "attack.h"
#include "bootstrap.h"

int main(int argc, char **argv){

    unsigned char ip[4];
    ip[0] = 129;
    ip[1] = 169;
    ip[2] = 159;
    ip[3] = 192;

    char ip_str[16];

    ip2srt(ip_str, ip);

    printf("ip : %s\n", ip_str);

    return 0;
}