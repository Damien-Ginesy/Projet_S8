#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip.h"
#include "attack.h"
#include "exchange_protocol.h"

struct attack_info *attacks_info = NULL;
int attacks_nbr = 0;

void attacks_store_by_mask(
    struct attack_info *attacks_tab,
    int attacks_tab_size
);

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
    
    attacks_store_by_mask(tab1, tab1_size);
    attacks_store_by_mask(tab2, tab2_size);

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

    attacks_store_by_mask(attacks_info, attacks_nbr);

    for(int attack_i = 0; attack_i < attacks_nbr; attack_i++){
        
        if(attacks_info[attack_i].mask == 0)
            continue;
        
        ip_alloc_network_ip(attacks_info[attack_i].network_ip, attacks_info[attack_i].mask);
        
    }

}

// attacks tab
void attacks_alloc_tab(int nbr_attacks){
    
    attacks_nbr = nbr_attacks;
    attacks_info = malloc(nbr_attacks*sizeof(struct attack_info));

    for(int i = 0; i < attacks_nbr; i++){
        attacks_info[i].mask = 0;
        memset(attacks_info[i].network_ip, 0, 4*sizeof(char));
        attacks_info[i].members = attack_members_list_new();
    }

}

void attacks_free_tab(){

    for(int i = 0; i < attacks_nbr; i++){
        attack_members_list_free(attacks_info[i].members);
    }

    free(attacks_info);

}

void attacks_set_id_by_index(int index, int id){
    attacks_info[index].id = id;
}
void attacks_set_mask_by_index(int index, int mask){
    attacks_info[index].mask = mask;
}

struct attack_info *attacks_get_attack_by_id(int id){
    for(int i = 0; i < attacks_nbr; i++){
        if(attacks_info[i].id == id){
            return attacks_info+i;
        }
    }

    fprintf(stderr, "attacks_get_attack_by_id : id not found\n");
    exit(EXIT_FAILURE);

}

// attackers list

struct attack_members_list *attack_members_list_new(){
    
    struct attack_members_list *aml = malloc(sizeof(struct attack_members_list *));
    
    aml->size = 0;
    aml->first_member = NULL;

    return aml;
}

void attack_member_free_recursif(struct attack_member *am){
    if(am->next != NULL){
        attack_member_free_recursif(am->next);
    }
    free(am);
}

void attack_members_list_free(struct attack_members_list *aml){
    attack_member_free_recursif(aml->first_member);
    free(aml);
}

void attack_members_add(
    struct attack_members_list *aml,
    struct node_network_info *member_net_info
){
    
    struct attack_member *member = malloc(sizeof(struct attack_member));

    member->net_info = member_net_info;
    member->next = aml->first_member;

    aml->first_member = member;

    aml->size++;
}


// height level

void attacks_get_net_ip_by_attack_id(unsigned char *net_ip, char *mask, int attack_id){
    
    struct attack_info *attack = attacks_get_attack_by_id(attack_id);

    memcpy(net_ip, attack->network_ip, 4*sizeof(char));
    *mask = attack->mask;

}

void attack_register_attacker(int attack_id, struct node_network_info *network_info){
    
    struct attack_info *attack = attacks_get_attack_by_id(attack_id);

    attack_members_add(attack->members, network_info);

}

void attack_get_attack_member_as_tab(
    struct node_network_info **attack_member_tab_out,
    int *attack_member_tab_size_out, 
    int attack_id,
    int requester_virtual_ip
){

    if(attack_id == 0){
        *attack_member_tab_out = NULL;
        *attack_member_tab_size_out = 0;
        return;
    }

    struct attack_info *attack = attacks_get_attack_by_id(attack_id);

    *attack_member_tab_size_out = attack->members->size -1;
    *attack_member_tab_out = malloc((*attack_member_tab_size_out) * sizeof(struct node_network_info));

    struct attack_member *member_in_list = attack->members->first_member;

    int tab_i = 0;
    for(int i = 0; i < (*attack_member_tab_size_out) +1; i++){

        if(member_in_list->net_info->virtual_ip == requester_virtual_ip){
            member_in_list = member_in_list->next;
            continue;
        }
        
        memcpy((*attack_member_tab_out)+tab_i, member_in_list->net_info, sizeof(struct node_network_info));
        tab_i++;

        member_in_list = member_in_list->next;

    }

}