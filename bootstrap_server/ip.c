#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "ip.h"

int mask1_allocated_adrr_size; // * 1 = size in char
unsigned char *mask1_allocated_adrr;
int mask1_allocated_adrr_curser; // size in ip

int mask2_allocated_adrr_size; // * 2 = size in char
unsigned char *mask2_allocated_adrr;
int mask2_allocated_adrr_curser; // size in ip

int mask3_allocated_adrr_size; // * 3 = size in char
unsigned char *mask3_allocated_adrr;
int mask3_allocated_adrr_curser; // size in ip

int mask4_allocated_adrr_size; // * 4 = size in char
unsigned char *mask4_allocated_adrr;
int mask4_allocated_adrr_curser; // size in ip 

void ip_alloc_init(int mask1_nbraddr, int mask2_nbraddr, int mask3_nbraddr, int mask4_nbraddr){
    
    mask1_allocated_adrr_size = mask1_nbraddr;
    mask1_allocated_adrr_curser = 0;
    if(mask1_nbraddr > 0){
        mask1_allocated_adrr = malloc(mask1_nbraddr * sizeof(char));
    }else{
        mask1_allocated_adrr = NULL;
    }

    mask2_allocated_adrr_size = mask2_nbraddr;
    mask2_allocated_adrr_curser = 0;
    if(mask2_nbraddr > 0){
        mask2_allocated_adrr = malloc(mask2_nbraddr * 2 * sizeof(char));
    }else{
        mask2_allocated_adrr = NULL;
    }

    mask3_allocated_adrr_size = mask3_nbraddr;
    mask3_allocated_adrr_curser = 0;
    if(mask3_nbraddr > 0){
        mask3_allocated_adrr = malloc(mask3_nbraddr * 3 * sizeof(char));
    }else{
        mask3_allocated_adrr = NULL;
    }

    mask4_allocated_adrr_size = mask4_nbraddr;
    mask4_allocated_adrr_curser = 0;
    if(mask4_nbraddr > 0){
        mask4_allocated_adrr = malloc(mask4_nbraddr * 4 * sizeof(char));
    }else{
        mask4_allocated_adrr = NULL;
    }

    srand(time(NULL));

}

void ip_alloc_free(){
    
    if(mask1_allocated_adrr != NULL){
        free(mask1_allocated_adrr);
    }

    if(mask2_allocated_adrr != NULL){
        free(mask2_allocated_adrr);
    }

    if(mask3_allocated_adrr != NULL){
        free(mask3_allocated_adrr);
    }

    if(mask4_allocated_adrr != NULL){
        free(mask4_allocated_adrr);
    }
}

void ip_alloc_get_registerd_net_ip_by_mask(
    unsigned char **mask_allocated_adrr,
    int *mask_allocated_adrr_curser,
    char mask
){

    switch (mask){

        case 4:
            
            *mask_allocated_adrr = mask4_allocated_adrr;
            *mask_allocated_adrr_curser = mask4_allocated_adrr_curser;
            
            
            break;

        case 3:
            
            *mask_allocated_adrr = mask3_allocated_adrr;
            *mask_allocated_adrr_curser = mask3_allocated_adrr_curser;
            
            
            break;

        case 2:
            
            *mask_allocated_adrr = mask2_allocated_adrr;
            *mask_allocated_adrr_curser = mask2_allocated_adrr_curser;
            
            
            break;

        case 1:
            
            *mask_allocated_adrr = mask1_allocated_adrr;
            *mask_allocated_adrr_curser = mask1_allocated_adrr_curser;
            
            
            break;
        
        default:

            fprintf(stderr, "ip_alloc_get_registerd_net_ip_by_mask : mask must be equal to 1, 2, 3 or 4\n");
            exit(EXIT_FAILURE);

            break;
    }

}

char ip_is_alloc(unsigned char *ip, char mask){

    unsigned char *mask_allocated_adrr;
    int mask_allocated_adrr_curser; // size in ip 

    for(int g_mask = 1; g_mask <= mask; g_mask++){
        
        ip_alloc_get_registerd_net_ip_by_mask(
            &mask_allocated_adrr,
            &mask_allocated_adrr_curser,
            g_mask
        );

        for(int i = 0; i < mask_allocated_adrr_curser; i++){
            if(memcmp(ip, mask_allocated_adrr+(g_mask*i), g_mask*sizeof(char)) == 0){
                return 1;
            }
        }
    }

    return 0;

}

void ip_alloc_network_ip(unsigned char *ip, char mask){

    unsigned char *mask_allocated_adrr;
    int mask_allocated_adrr_curser; // size in ip 

    ip_alloc_get_registerd_net_ip_by_mask(
        &mask_allocated_adrr,
        &mask_allocated_adrr_curser,
        mask
    );

    memset(ip, 0, 4*sizeof(char));

    while(1){

        // choosing_rand
        for(int i = 4-mask; i < 4; i++){
            ip[i] = rand() % 255;
        }

        if(!ip_is_alloc(ip, mask)){
            break;
        }
    }

    

    memcpy(mask_allocated_adrr+mask*mask_allocated_adrr_curser, ip+(4-mask), mask*sizeof(char));
    mask_allocated_adrr_curser++;

}

void ip_alloc(unsigned char *ip_out, unsigned char *net_ip, char mask){

    for(int i = 4-mask; i < 4; i++){
       ip_out[i] = net_ip[i];
    }

    char already_alloc = 1;
    while(already_alloc){

        for(int i = 0; i < 4-mask; i++){
            ip_out[i] = rand() % 255;
        }

        already_alloc = ip_is_alloc(ip_out, 4);

    }

    memcpy(mask4_allocated_adrr + 4*mask4_allocated_adrr_curser, ip_out, 4*sizeof(char));
    mask4_allocated_adrr_curser++;

}


void ip_int2ip(int ip_int, unsigned char *ip){
    
    for(int i = 0; i < 4; i++){
        ip[i] = (unsigned char) (ip_int>>(i*8));
    }

}

int ip_ip2int(unsigned char *ip){
    return *((int *)ip);
}

int ip_str2intip(char *str){
    
    unsigned char ip[4];
    int ip_index = 3;

    char nbr_buffer[3];
    int nbr_buffer_index = 0;

    char c;

    for(int i = 0; i < strlen(str); i++){
        c = str[i];

        if(c == '.'){
            nbr_buffer[nbr_buffer_index++] = '\0';
            ip[ip_index--] = (unsigned char) atoi(nbr_buffer);
            nbr_buffer_index = 0;
        }else{
            nbr_buffer[nbr_buffer_index++] = c;
        }
    }

    nbr_buffer[nbr_buffer_index++] = '\0';
    ip[ip_index--] = (unsigned char) atoi(nbr_buffer);

    return ip_ip2int(ip);
}

void ip2srt(char *str_out, unsigned char *ip){
    sprintf(
        str_out,
        "%d.%d.%d.%d",
        ip[3], ip[2], ip[1], ip[0]
    );
}

void ip_print(unsigned char *ip){
    for(int i = 3; i > 0; i--){
        printf("%d.", ip[i]);
    }
    printf("%d", ip[0]);
    
}

void ip_and_port_from_struct_sockaddr(
    struct sockaddr sockaddr_in,
    int *ip_out,
    uint16_t *port_out
){
    
    // ip
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&sockaddr_in;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );

    *ip_out = ip_str2intip(str);

    // port
    *port_out = pV4Addr->sin_port;

}

// Test

void ip_print_after_init(){
    printf("mask1_allocated_adrr_size = %d\n", mask1_allocated_adrr_size);
    printf("mask2_allocated_adrr_size = %d\n", mask2_allocated_adrr_size);
    printf("mask3_allocated_adrr_size = %d\n", mask3_allocated_adrr_size);
    printf("mask4_allocated_adrr_size = %d\n", mask4_allocated_adrr_size);
}

void ip_test_conv(){
    unsigned char ip[4];
    
    int x = -1;
    printf("%d\n", x);
    
    ip_int2ip(x, ip);
    ip_print(ip);
    printf("\n");
    
    printf("%d\n", ip_ip2int(ip));
}

void ip_test_alloc_net_ip(){

    unsigned char ip[4];

    ip_alloc_init(1, 1, 1, 1);

    ip_alloc_network_ip(ip, 1);
    ip_print(ip);
    printf("\n");

    ip_alloc_network_ip(ip, 2);
    ip_print(ip);
    printf("\n");

    ip_alloc_network_ip(ip, 3);
    ip_print(ip);
    printf("\n");

    ip_alloc_network_ip(ip, 4);
    ip_print(ip);
    printf("\n");

    ip_alloc_free();

}

void ip_test_alloc_ip(){

    unsigned char ip_net[4];
    unsigned char ip[4];

    ip_alloc_init(1, 1, 4, 10);

    printf("net ip :\n");

    ip_alloc_network_ip(ip_net, 3);
    ip_print(ip_net);
    printf("\n");

    printf("ip :\n");

    for(int i = 0; i < 5; i++){
        ip_alloc(ip, ip_net, 3);
        ip_print(ip);
        printf("\n");
    }


    ip_alloc_free();

}

void ip_final_test(){

    ip_alloc_init(0, 1, 2, 10000);

    unsigned char ip_net_mask2[4];
    unsigned char ip_net_mask3_1[4];
    unsigned char ip_net_mask3_2[4];

    unsigned char ip[4];

    printf("net ip :\n");
    ip_alloc_network_ip(ip_net_mask2, 3);
    ip_print(ip_net_mask2);
    printf(" adresses :\n");
    for(int i = 0; i < 400; i++){
        ip_alloc(ip, ip_net_mask2, 2);
        ip_print(ip);
        printf("\n");
    }

    printf("net ip :\n");
    ip_alloc_network_ip(ip_net_mask3_1, 3);
    ip_print(ip_net_mask3_1);
    printf(" adresses :\n");
    for(int i = 0; i < 150; i++){
        ip_alloc(ip, ip_net_mask3_1, 3);
        ip_print(ip);
        printf("\n");
    }

    printf("net ip :\n");
    ip_alloc_network_ip(ip_net_mask3_2, 3);
    ip_print(ip_net_mask3_2);
    printf(" adresses :\n");
    for(int i = 0; i < 100; i++){
        ip_alloc(ip, ip_net_mask3_2, 3);
        ip_print(ip);
        printf("\n");
    }

    printf("other ip :\n");
    for(int i = 0; i < 10000-700; i++){
        ip_alloc(ip, NULL, 0);
        ip_print(ip);
        printf("\n");
    }


    ip_alloc_free();

}