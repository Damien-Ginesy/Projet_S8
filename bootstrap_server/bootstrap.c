/*******************************************************************************
 * This file is part of Basalt Project.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "bootstrap.h"
#include "attack.h"

int node_nbr = 0;
struct node_info *node_tab = NULL;
int node_current = 0;

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

    if(argc < 2){
        fprintf(stderr, "No argument where given. Running Example : \n\t./bootstrap_server eclipse 1 inst 2 24 10000\n");
        exit(EXIT_FAILURE);
    }

    attacks_alloc_tab(compute_attacks_nbr(argc, argv));
    

    int nbr_net_ip_by_mask[3];
    nbr_net_ip_by_mask[0] = 0;
    nbr_net_ip_by_mask[1] = 0;
    nbr_net_ip_by_mask[2] = 0;

    int natural_mask; // 8 16 24
    int arg_i = 1; // ignoring executable name
    int attack_i = 0;
    while(1){
        if(strcmp(argv[arg_i], "eclipse") == 0){
            arg_i++;
            
            attacks_set_id_by_index(attack_i, atoi(argv[arg_i++]));

            attack_i++;

        }else if(strcmp(argv[arg_i], "inst") == 0){
            arg_i++;

            attacks_set_id_by_index(attack_i, atoi(argv[arg_i++]));

            natural_mask = atoi(argv[arg_i++]);
            

            switch (natural_mask){
                
                case 8:
                    nbr_net_ip_by_mask[0]++;
                    attacks_set_mask_by_index(attack_i, 1);
                    break;

                case 16:
                    nbr_net_ip_by_mask[1]++;
                    attacks_set_mask_by_index(attack_i, 2);
                    break;
                
                case 24:
                    nbr_net_ip_by_mask[2]++;
                    attacks_set_mask_by_index(attack_i, 3);
                    break;
                
                default:
                    fprintf(stderr, "mask not known. known masks : 8, 16 and 24.\n");
                    exit(EXIT_FAILURE);
                    break;
            }

            attack_i++;

        }else{ // nbr nodes

            node_nbr = atoi(argv[arg_i]);

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

    node_tab = malloc(node_nbr * sizeof(struct node_info));

}

void test_parse_simu_params(int argc, char **argv){
    parse_simu_params(argc, argv);
    ip_print_after_init();
}
