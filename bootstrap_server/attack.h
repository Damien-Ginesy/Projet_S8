#ifndef _ATTACK_H_
#define _ATTACK_H_


struct attack_member{
    struct node_network_info * net_info;
    struct attackers_list *next;
};

struct attack_members_list{
    int size;
    struct attack_member *first_member;
};


struct attack_info{
    int id;
    char mask;
    unsigned char network_ip[4];
    struct attack_members_list *members;
};

extern int attacks_nbr;
extern struct attack_info *attacks_info;


void attacks_store_by_mask(
    struct attack_info *attacks_tab,
    int attacks_tab_size
);

// attacks tab
void attacks_alloc_tab(int nbr_attacks);
void attacks_free_tab(int nbr_attacks);

// attackers list

struct attack_members_list * attack_members_list_new();
void attack_members_list_free(struct attack_members_list *aml);




#endif