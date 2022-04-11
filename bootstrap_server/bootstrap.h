#ifndef BOOTSTRAP_H_
#define BOOTSTRAP_H_

#include "exchange_protocol.h"

struct node_info{
    struct node_network_info network;
    int view_size;
    int attaque_id;
};

struct attack_info{
    int id;
    char mask;
    unsigned char network_ip[4];
    int count_member;
    struct node_network_info *members;
};

// simu params
int node_nbr;
int attacks_nbr;
struct attack_info *attacks_info;

void parse_simu_params(int argc, int argv);

#endif