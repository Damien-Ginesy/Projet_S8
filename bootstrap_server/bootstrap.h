#ifndef BOOTSTRAP_H_
#define BOOTSTRAP_H_

#include "exchange_protocol.h"
#include "ip.h"

struct node_info{
    struct node_network_info *network;
    int view_size;
    int attaque_id;
};

// simu params
extern int node_nbr;
extern struct node_info *node_tab;
extern int node_current;

void parse_simu_params(int argc, char **argv);

// Tests
void test_parse_simu_params(int argc, char **argv);


// // add node_network_info data
// struct node_network_info *add_node_network_info(int ip,unsigned short port,int virtual_ip);

// // intialisation of the chain
// attack_point *initialisation();

// // insertion in chain list
// void attacker_add_list(attack_point *attack_point,struct node_network_info *attacker_info);

// // add node_info data
// struct node_info *add_node_info(struct node_network_info *network,int view_size,int attack_id);

// // update attack_info
// void update_attacks_info(struct node_network_info *network, int attack_id);

// // print attacker list
// void print_list_attackers();

#endif