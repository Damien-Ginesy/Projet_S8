#ifndef _ATTACK_H_
#define _ATTACK_H_


struct attack_member{
    struct node_network_info * net_info;
    struct attack_member *next;
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


void attack_generate_net_ip();

// attacks tab
void attacks_alloc_tab(int nbr_attacks);
void attacks_free_tab();

void attacks_set_id_by_index(int index, int id);
void attacks_set_mask_by_index(int index, int mask);


// attackers list

struct attack_members_list * attack_members_list_new();
void attack_members_list_free(struct attack_members_list *aml);




#endif