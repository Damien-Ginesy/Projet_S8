#ifndef __EXCHANGE_PROTOCOL_H__
#define __EXCHANGE_PROTOCOL_H__

#include <inttypes.h>

/* --- Exchange protocol ---

    Basalt Node -> Bootstrap server :

        Sending struct bootstrap_req.

    Bootstrap server ->  Basalt Node :

        Sending bootstrap_res.
        Sending view (table of  struct node_network_info) if view_size > 0.
        Sending malicious view (table of struct node_network_info) if malicious_view_size > 0.

*/

typedef struct node_network_info{
    uint32_t ip;
    uint16_t port;
    uint32_t virtual_ip;
} node_network_info;

typedef struct bootstrap_req{
    uint32_t view_size;
    uint16_t port;
    uint32_t attack_id; // = 0 for non malicious node
} bootstrap_req;

typedef struct bootstrap_res{
    uint32_t ip;
    uint32_t real_ip;
    uint32_t view_size;
    uint32_t malicious_view_size; 
} bootstrap_res;


#endif