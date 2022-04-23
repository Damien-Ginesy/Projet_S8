/*******************************************************************************
 * This file is part of Basalt Project.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "bootstrap.h"

// ----- parse_simu_params

int compute_attacks_nbr(int argc, char **argv){
    int comp = 0;
    for(int arg_i = 1; arg_i < argc; arg_i++){
        if(strcmp(argv[arg_i], "eclipse") == 0 || strcmp(argv[arg_i], "inst") == 0){
            comp++;
        }
    }
    return comp;
}

void parse_simu_params(int argc, char **argv){

    attacks_nbr = compute_attacks_nbr(argc, argv);
    printf("attacks_nbr = %d\n", attacks_nbr);
    
    attacks_info = malloc(attacks_nbr*sizeof(struct attack_info));
    
    for(int i = 0; i < attacks_nbr; i++){
        attacks_info[i].mask = 0;
        attacks_info[i].count_member = 0;
        attacks_info[i].attack_point = NULL;
    }

    int nbr_net_ip_by_mask[3];
    nbr_net_ip_by_mask[0] = 0;
    nbr_net_ip_by_mask[1] = 0;
    nbr_net_ip_by_mask[2] = 0;

    int arg_i = 1; // ignoring executable name
    int attack_i = 0;
    while(1){
        if(strcmp(argv[arg_i], "eclipse") == 0){
            arg_i++;

            attacks_info[attack_i].id = atoi(argv[arg_i++]);

            attack_i++;

        }else if(strcmp(argv[arg_i], "inst") == 0){
            arg_i++;

            attacks_info[attack_i].id = atoi(argv[arg_i++]);

            attacks_info[attack_i].mask = atoi(argv[arg_i++]);
            
            //ip_alloc_network_ip(attacks_info[attack_i].network_ip, attacks_info[attack_i].mask);

            switch (attacks_info[attack_i].mask){
                
                case 8:
                    nbr_net_ip_by_mask[0]++;
                    break;

                case 16:
                    nbr_net_ip_by_mask[1]++;
                    break;
                
                case 24:
                    nbr_net_ip_by_mask[2]++;
                    break;
                
                default:
                    fprintf(stderr, "mask not known. known masks : 8, 16 and 24.\n");
                    exit(EXIT_FAILURE);
                    break;
            }

            attack_i++;

        }else{ // nbr nodes

            node_nbr = atoi(argv[arg_i]);
            printf("node_nbr = %s\n", argv[arg_i]);

            if(node_nbr == 0){
                fprintf(stderr, "Non valid params : Node_nbr = %s\n", argv[arg_i]);
                exit(EXIT_FAILURE);
            }

            if(arg_i+1 > argc){
                fprintf(stderr, "Non valid params : arg_i+1 >= argc\n");
                exit(EXIT_FAILURE);
            }

            break;

        }
    }
    
    ip_alloc_init(nbr_net_ip_by_mask[0], nbr_net_ip_by_mask[1], nbr_net_ip_by_mask[2], node_nbr);

}

void test_parse_simu_params(int argc, char **argv){
    parse_simu_params(argc, argv);
    ip_print_after_init();
}

// initialisation
attack_point *initialisation()
{
    attack_point *attack_point = malloc(sizeof(*attack_point));
    if (attack_point == NULL)
    {
        exit(EXIT_FAILURE);
    }

    attack_point->premier = NULL;

    return attack_point;
}

// attacker_add_list in chain list
void attacker_add_list(attack_point *attack_point,struct node_network_info *attacker_info)
{
	// new attacker creation
    attackers_list *new_attacker = malloc(sizeof(*new_attacker));
    if (new_attacker == NULL)
    {
        exit(EXIT_FAILURE);
    }

	new_attacker->attacker_info = attacker_info;
        

	// attacker_add_list in the chain
		new_attacker->suivant = attack_point->premier;
        attack_point->premier = new_attacker;
		
    
}

// add node_network_info data
struct node_network_info *add_node_network_info(int ip,unsigned short port,int virtual_ip)
{
    // new attacker creation
    struct node_network_info *node_network_info = malloc(sizeof(*node_network_info));
    if (node_network_info == NULL)
    {
        exit(EXIT_FAILURE);
    }
    node_network_info->ip = ip;
    node_network_info->port = port;
    node_network_info->virtual_ip = virtual_ip;
    
    return node_network_info;
}

// add node_info data
struct node_info *add_node_info(struct node_network_info *network,int view_size,int attack_id)
{
    // new attacker creation
    struct node_info *node_info = malloc(sizeof(*node_info));
    if (node_info == NULL)
    {
        exit(EXIT_FAILURE);
    }
    node_info->attaque_id = attack_id;
    node_info->network = network;
    node_info->view_size = view_size;
    return node_info;
}

// update attack_info
void update_attacks_info(struct node_network_info *network, int attack_id)
{
    for(int i = 0; i < attacks_nbr; i++){
        if (attack_id == attacks_info[i].id)
        {
            attacks_info[i].count_member++;
            attacker_add_list(attacks_info[i].attack_point,network);
        }
    }    
}

//check out attacker list

void print_list_attackers()
{   
    for(int i = 0; i < attacks_nbr; i++){
        if ( attacks_info[i].attack_point->premier != NULL){
            attackers_list* attacker_actuel = attacks_info[i].attack_point->premier;
            while (attacker_actuel->suivant != NULL)
            {				
                printf("attacker info : IP : %i, PORT : %i , VIRTUAL IP : %i \n", attacker_actuel->attacker_info->ip,attacker_actuel->attacker_info->port,attacker_actuel->attacker_info->virtual_ip);
                attacker_actuel = attacker_actuel->suivant;
            }	
	    }
    } 
}

// -----
