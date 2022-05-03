#include <stdio.h>

#include "ip.h"
#include "attack.h"
#include "bootstrap.h"

int main(int argc, char **argv){


    char ip_str[16];

    int ip_int = ip_str2intip("255.1.102.37");

    unsigned char ip[4];
    ip_int2ip(ip_int, ip);
    ip2srt(ip_str, ip);

    printf("ip : %s\n", ip_str);

    return 0;
}