#include <stdio.h>
#include "socket_IO.h"
#include "exchange_protocol.h"

#include "ip.h" //used only for printing received data

int main(int argc, char  *argv[]){
	if (argc != 5) {
		printf("Usage: ./client hostname port_number attack_id view_size\n");
		exit(EXIT_FAILURE);
	}
	char *hostname = argv[1];
	char *port = argv[2];
    char ip_str[1024];
    memset(ip_str,0,1024);

	int sock_fd = -1;
	if (-1 == (sock_fd = socket_and_connect(hostname, port,ip_str))) {
		printf("Could not create socket and connect properly\n");
		return 1;
	}

    struct bootstrap_req bootstrap_req;
    bootstrap_req.attack_id = atoi(argv[3]);

    /*
        ip and port are obtined directely by the server at the accept.
        So there is no need to correctly specify them.
    */
    bootstrap_req.ip = 0;
    bootstrap_req.port = 0;

    bootstrap_req.view_size = atoi(argv[4]);


    // Send Data to Node
    send_data(sock_fd,(char*) &bootstrap_req,sizeof(bootstrap_req),"sending bootstrap_req");
    printf("Data sent !\n");

    // Receiving data
    struct bootstrap_res bootstrap_res;
    receive_data(sock_fd, (char*) &bootstrap_res, sizeof(bootstrap_res), "receiving bootstrap_res");

    struct node_network_info *view = NULL;
    struct node_network_info *malicious_view = NULL;

    if(bootstrap_res.view_size > 0){
        view = malloc(bootstrap_res.view_size * sizeof(struct node_network_info));
        receive_data(
            sock_fd,
            (char*) view,
            bootstrap_res.view_size * sizeof(struct node_network_info),
            "receiving view"
        );
    }

    if(bootstrap_res.malicious_view_size > 0){
        malicious_view = malloc(bootstrap_res.malicious_view_size * sizeof(struct node_network_info));
        receive_data(
            sock_fd,
            (char*) malicious_view,
            bootstrap_res.malicious_view_size * sizeof(struct node_network_info),
            "receiving malicious_view"
        );
    }

    // print data
    {
        char ip_str[16];
        unsigned char ip[4];
        char vip_str[16];
        unsigned char vip[4];

        // -- view
        printf("View :\n");
        for(int vi = 0; vi < bootstrap_res.view_size; vi++){

            ip_int2ip(view[vi].ip , ip);
            ip_int2ip(view[vi].virtual_ip , vip);

            ip2srt(ip_str, ip);
            ip2srt(vip_str, vip);

            printf("\t%s\t%s:%hu\n", vip_str, ip_str, view[vi].port);

        }

        // malicious view
        printf("Malicous View :\n");
        for(int mvi = 0; mvi < bootstrap_res.malicious_view_size; mvi++){

            ip_int2ip(malicious_view[mvi].ip , ip);
            ip_int2ip(malicious_view[mvi].virtual_ip , vip);

            ip2srt(ip_str, ip);
            ip2srt(vip_str, vip);

            printf("\t%s\t%s:%hu\n", vip_str, ip_str, malicious_view[mvi].port);

        }
    }

    // free
    if(view != NULL){
        free(view);
    }
    if(malicious_view != NULL){
        free(malicious_view);
    }

    return 0;
}
