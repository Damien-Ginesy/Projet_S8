#include <stdio.h>

#include "ip.h"
#include "attack.h"
#include "bootstrap.h"

int main(int argc, char **argv){

    parse_simu_params(argc, argv);
    attack_generate_net_ip();

    return 0;
}