#include <stdio.h>
#include "socket_IO.h"
#include "bootstrap.h"
#include "exchange_protocol.h"

int main(int argc, char  *argv[]) {
	if (argc != 3) {
		printf("Usage: ./client hostname port_number\n");
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
    bootstrap_req.attack_id = 0;
    bootstrap_req.ip = 64550;
    bootstrap_req.port = 8080;
    bootstrap_req.view_size = 100000;


    // Send Data to Node
    send_data(sock_fd,(char*) &bootstrap_req,sizeof(bootstrap_req),"data to server");
    printf("Data sent !\n");
    sleep(10);
    return 0;
}
