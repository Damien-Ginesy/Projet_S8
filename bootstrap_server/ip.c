#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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



void ip_alloc_network_ip(unsigned char *ip, char mask){

    memset(ip, 0, 4*sizeof(char));
    
    char already_allocated;
    switch (mask){

        case 4:

            already_allocated = 1;
            while(already_allocated){

                already_allocated = 0;

                // choosing_rand
                for(int i = 3; i >= 0; i-- ){
                    ip[i] = rand() % 255;
                }

                // verifying if already allocated
                for(int i = 0; i < mask4_allocated_adrr_curser; i++){
                    if(memcmp(ip, mask4_allocated_adrr+(4*i), 4*sizeof(char))){
                        already_allocated = 1;
                        break;
                    }
                }

            }

            // adding ip in allocation list
            memcpy(mask4_allocated_adrr+(4*mask4_allocated_adrr_curser), ip, 4*sizeof(char));
            mask4_allocated_adrr_curser++;

            break;

        case 3:

            already_allocated = 1;
            while(already_allocated){

                already_allocated = 0;

                // choosing_rand
                for(int i = 3; i >= 1; i-- ){
                    ip[i] = rand() % 255;
                }

                // verifying if already allocated
                for(int i = 0; i < mask3_allocated_adrr_curser; i++){
                    if(memcmp(ip, mask3_allocated_adrr+(3*i), 3*sizeof(char))){
                        already_allocated = 1;
                        break;
                    }
                }

                

            }

            // adding ip in allocation list
            memcpy(mask3_allocated_adrr+(3*mask3_allocated_adrr_curser), ip, 3*sizeof(char));
            mask3_allocated_adrr_curser++;

            break;

        case 2:

            already_allocated = 1;
            while(already_allocated){

                already_allocated = 0;

                // choosing_rand
                for(int i = 3; i >= 2; i-- ){
                    ip[i] = rand() % 255;
                }

                // verifying if already allocated
                for(int i = 0; i < mask2_allocated_adrr_curser; i++){
                    if(memcmp(ip, mask2_allocated_adrr+(2*i), 2*sizeof(char))){
                        already_allocated = 1;
                        break;
                    }
                }

                

            }

            // adding ip in allocation list
            memcpy(mask2_allocated_adrr+(2*mask2_allocated_adrr_curser), ip, 2*sizeof(char));
            mask2_allocated_adrr_curser++;

            break;

        case 1:

            already_allocated = 1;
            while(already_allocated){

                already_allocated = 0;

                // choosing_rand
                for(int i = 3; i >= 3; i-- ){
                    ip[i] = rand() % 255;
                }

                // verifying if already allocated
                for(int i = 0; i < mask1_allocated_adrr_curser; i++){
                    if(memcmp(ip, mask1_allocated_adrr+(1*i), 1*sizeof(char))){
                        already_allocated = 1;
                        break;
                    }
                }

                

            }

            // adding ip in allocation list
            memcpy(mask1_allocated_adrr+(1*mask1_allocated_adrr_curser), ip, 1*sizeof(char));
            mask1_allocated_adrr_curser++;

            break;
        
        default:
            fprintf(stderr, "ip_alloc_network_ip : mask must be equal to 1, 2, 3 or 4\n");
            exit(EXIT_FAILURE);
            break;
    }
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

void ip_test_conv(){
    unsigned char ip[4];
    
    int x = -1;
    printf("%d\n", x);
    
    ip_int2ip(x, ip);
    ip_print(ip);
    printf("\n");
    
    printf("%d\n", ip_ip2int(ip));
}

void ip_test_alloc(){

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