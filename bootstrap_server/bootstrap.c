/*******************************************************************************
 * This file is part of Basalt Project.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "bootstrap.h"

// ----- parse_simu_params

// int compute_attacks_nbr(int argc, int **argv){
//     int comp = 0;
//     for(int arg_i = 1; arg_i < argc; arg_i++){
//         if(strcmp(argv[arg_i], "eclipse") == 0 || strcmp(argv[arg_i], "inst") == 0){
//             comp++;
//         }
//     }
//     return comp;
// }

// void parse_simu_params(int argc, int **argv){

//     attacks_nbr = compute_attacks_nbr(argc, argv);
    
//     attacks_info = malloc(attacks_nbr*sizeof(struct attack_info));
//     for(int i = 0; i < attacks_nbr; i++){
//         attacks_info[i].mask = 0;
//         attacks_info[i].count_member = 0;
//         attacks_info[i].members = NULL;
//     }

//     int arg_i = 0;
//     int attack_i = 0;
//     while(1){
//         if(strcmp(argv[arg_i], "eclipse") == 0){
//             arg_i++;

//             attacks_info[attack_i].id = atoi(argv[arg_i++]);

//             attack_i++;

//         }else if(strcmp(argv[arg_i], "inst") == 0){
//             arg_i++;

            

//             attack_i++;

//         }else{ // nbr nodes

//             node_nbr = atoi(argv[arg_i]);

//             if(node_nbr == 0){
//                 fprintf(stderr, "Non valid params\n");
//                 exit(EXIT_FAILURE);
//             }

//             if(arg_i+1 >= argc){
//                 fprintf(stderr, "Non valid params\n");
//                 exit(EXIT_FAILURE);
//             }

//             break;

//         }
//     }
// }

// -----
