#ifndef __EXCHANGE_PROTOCOL_H__
#define __EXCHANGE_PROTOCOL_H__

/* --- Exchange protocol ---

    Basalt Node -> Bootstrap server :

        Sending struct bootstrap_req.

    Bootstrap server ->  Basalt Node :

        Sending bootstrap_res.
        Sending view (table of  struct node_network_info) if view_size > 0.
        Sending malicious view (table of struct node_network_info) if malicious_view_size > 0.

*/

struct node_network_info{
    int ip;
    unsigned short port;
    int virtual_ip;
};

struct bootstrap_req{
    unsigned int view_size;
    int ip;
    unsigned short port;
    unsigned int attack_id; // = 0 for non malicious node
};

struct bootstrap_res{
    int ip;
    unsigned int view_size;
    unsigned int malicious_view_size; 
};


#endif