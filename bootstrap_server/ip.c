#include <stdio.h>

#include "ip.h"

void ip_int2ip(int ip_int, unsigned char *ip){
    
    for(int i = 0; i < 4; i++){
        ip[3-i] = (unsigned char) (ip_int>>(i*8));
    }

}

void ip_print(unsigned char *ip){
    for(int i = 0; i < 3; i++){
        printf("%d.", ip[i]);
    }
    printf("%d", ip[3]);
    
}

void ip_test(){
    unsigned char ip[4];
    ip_int2ip(-1, ip);
    ip_print(ip);
    printf("\n");
}