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
        attacks_info[i].members = NULL;
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

// -----
