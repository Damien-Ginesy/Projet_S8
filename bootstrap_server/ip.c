#include <stdio.h>
#include <stdlib.h>

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

unsigned char ip_alloc_ip_with_mask(int mask){

}

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
    
    int x = -1;
    printf("%d\n", x);
    
    ip_int2ip(x, ip);
    ip_print(ip);
    printf("\n");
    
    printf("%d\n", ip_ip2int(ip));
}