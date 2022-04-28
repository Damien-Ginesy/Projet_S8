#ifndef BOOTSTRAP_H_
#define BOOTSTRAP_H_

#include "exchange_protocol.h"
#include "ip.h"

struct node_info{
    struct node_network_info network;
    int view_size;
    int attaque_id;
};

// simu params
extern int node_nbr;
extern struct node_info *node_tab;
extern int node_current;

void parse_simu_params(int argc, char **argv);

void generate_view(int view_size, struct node_network_info *view);

// Tests
void test_parse_simu_params(int argc, char **argv);


#endif