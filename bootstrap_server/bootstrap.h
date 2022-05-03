#ifndef BOOTSTRAP_H_
#define BOOTSTRAP_H_

#include "exchange_protocol.h"
#include "ip.h"

struct node_info{
    struct node_network_info *network;
    int view_size;
    int attaque_id;
};

typedef struct attackers_list attackers_list;
struct attackers_list{
    struct node_network_info * attacker_info;
    struct attackers_list *suivant;
};

typedef struct attack_point attack_point;
struct attack_point{
    struct attackers_list *premier;
};

struct attack_info{
    int id;
    char mask;
    unsigned char network_ip[4];
    int count_member;
    attack_point *attack_point;
};

// simu params
extern int node_nbr;
extern int attacks_nbr;
extern struct attack_info *attacks_info;



void parse_simu_params(int argc, char **argv);

void attack_list_store_by_mask(
    struct attack_info *attacks_tab,
    int attacks_tab_size
);

void attack_generate_net_ip();

// Tests
void test_parse_simu_params(int argc, char **argv);


// add node_network_info data
struct node_network_info *add_node_network_info(int ip,unsigned short port,int virtual_ip);

// intialisation of the chain
attack_point *initialisation();

// insertion in chain list
void attacker_add_list(attack_point *attack_point,struct node_network_info *attacker_info);

// add node_info data
struct node_info *add_node_info(struct node_network_info *network,int view_size,int attack_id);

// update attack_info
void update_attacks_info(struct node_network_info *network, int attack_id);

// print attacker list
void print_list_attackers();

#endif