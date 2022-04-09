#include <stdio.h>

#include "ip.h"

void ip_int2ip(int ip_int, unsigned char *ip){
    
    for(int i = 0; i < 4; i++){
        ip[i] = (unsigned char) (ip_int>>(i*8));
    }

}

int ip_ip2int(unsigned char *ip){
    return *((int *)ip);
}

void ip_print(unsigned char *ip){
    for(int i = 3; i > 0; i--){
        printf("%d.", ip[i]);
    }
    printf("%d", ip[0]);
    
}

void ip_test(){
    unsigned char ip[4];
    
    int x = 6512;
    printf("%d\n", x);
    
    ip_int2ip(x, ip);
    ip_print(ip);
    printf("\n");
    
    printf("%d\n", ip_ip2int(ip));
}