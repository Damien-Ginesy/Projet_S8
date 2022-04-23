#include <stdlib.h>

#include "attack.h"

void attacks_store_by_mask(
    struct attack_info *attacks_tab,
    int attacks_tab_size
){

    if(attacks_tab_size <= 1){
        return;
    }

    int m = attacks_tab_size/2;

    struct attack_info stored_tab_tmp[attacks_tab_size];
    struct attack_info *tab1 = attacks_tab;
    struct attack_info *tab2 = attacks_tab+m;
    int tab1_size = m;
    int tab2_size = attacks_tab_size - m;
    int tab1_index = 0;
    int tab2_index = 0;
    
    attack_list_store_by_mask(tab1, tab1_size);
    attack_list_store_by_mask(tab2, tab2_size);

    //fusion
    for(int i = 0; i < attacks_tab_size; i++){
        if(tab1_index >= tab1_size){
            stored_tab_tmp[i] = tab2[tab2_index];
            tab2_index++;
            continue;
        }
        if(tab2_index >= tab2_size){
            stored_tab_tmp[i] = tab1[tab1_index];
            tab1_index++;
            continue;
        }
        if(tab1[tab1_index].mask < tab2[tab2_index].mask){
            stored_tab_tmp[i] = tab1[tab1_index++];
        }else{
            stored_tab_tmp[i] = tab2[tab2_index++];
        }
    }

    memcpy(attacks_tab, stored_tab_tmp, attacks_tab_size*sizeof(struct attack_info));
    
}

void attack_generate_net_ip(){

    attack_list_store_by_mask(attacks_info, attacks_nbr);

    for(int attack_i = 0; attack_i < attacks_nbr; attack_i++){
        
        if(attacks_info[attack_i].mask == 0)
            continue;
        // J'AI REMPLACER LE attacks_info[attack_i].mask PAR 1 POUR TESTER CAR YAVAIS ERREUR MASK 2 24.. ALORS IL FAUT 1 2 3 4....
        ip_alloc_network_ip(attacks_info[attack_i].network_ip,1);
        
    }

}

// attackers list

struct attack_members_list *attack_members_list_new(){
    
    struct attack_members_list *aml = malloc(sizeof(struct attack_members_list *));
    
    aml->size = 0;
    aml->first_member = NULL;

    return aml;
}

void attack_member_free_recursif(struct attack_member *am){
    attack_member_free_recursif(am->next);
    free(am);
}

void attack_members_list_free(struct attack_members_list *aml){
    attack_member_free_recursif(aml->first_member);
    free(aml);
}