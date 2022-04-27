#include <stdio.h>
#include "socket_IO.h"
#include "bootstrap.h"
#include "exchange_protocol.h"
#include "attack.h"


int main(int argc, char *argv[]) {

/**** Parse simulation parametres and init db struct ******/
parse_simu_params(argc, argv);
attack_generate_net_ip();

/**********************************************************/

/****************** Listening socket ************************/
// Create listening socket
	char *port = "8080";
	int listen_fd = -1;
	if (-1 == (listen_fd = socket_listen_and_bind(node_nbr,port))) {
		printf("Could not create, bind and listen properly\n");
		return 1;
	}


/**********************************************************/

/*********** Data read and update and generate IP *********/



	// Handle new connections and existing ones
	
	// Declare array of struct pollfd
	struct pollfd pollfds[node_nbr+1];

	// Init first slot with listening socket
	pollfds[0].fd = listen_fd;
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	// Init remaining slot to default values
	for (int i = 1; i < node_nbr+1; i++) {
		pollfds[i].fd = -1;
		pollfds[i].events = 0;
		pollfds[i].revents = 0;
	}

   struct bootstrap_req bootstrap_req;
   struct bootstrap_res bootstrap_res;

   unsigned char net_ip[4];
   int mask;
   unsigned char ip[4];
   int virtual_ip;


   //attack_point *attack_point = initialisation();
   while(1){
      printf("pass la \n");
      int n_active =0;
         // Block until new activity detected on existing socket
		if (-1 == (n_active = poll(pollfds, node_nbr+1, -1))) {
			perror("Poll");
		}
		printf("[SERVER] : %d active socket\n", n_active);

         
      for (int i = 0; i < node_nbr+1; i++){
         // If listening socket is active => accept new incoming connection
         if (pollfds[i].fd == listen_fd && pollfds[i].revents & POLLIN) {
            // accept new connection and retrieve new socket file descriptor
            struct sockaddr client_addr;
            socklen_t size = sizeof(client_addr);
            int client_fd;
            if (-1 == (client_fd = accept(listen_fd, &client_addr, &size))) {
               perror("Accept");
            }
            printf("Node Accepted !\n");
            
            // store new file descriptor in available slot in the array of struct pollfd set .events to POLLIN
            for (int j = 0; j < node_nbr+1; j++) {
               if (pollfds[j].fd == -1) {
                  pollfds[j].fd = client_fd;
                  pollfds[j].events = POLLIN;
                  break;
               }
            }
            // clean memory
            memset(&bootstrap_req,0,sizeof(bootstrap_req));
            memset(&bootstrap_res,0,sizeof(bootstrap_res));

            // Recieve Data from Nodes
               receive_data(client_fd,(char*) &bootstrap_req,sizeof(bootstrap_req),"data from node");
               printf("[TEST] : Attaque ID : %i, Adresse IP : %i, Port : %hu, ViewSize : %i\n",bootstrap_req.attack_id,bootstrap_req.ip,bootstrap_req.port,bootstrap_req.view_size);
            
            // Saving node info

            node_tab[node_current].attaque_id = bootstrap_req.attack_id;
            node_tab[node_current].view_size = bootstrap_req.view_size;

            ip_and_port_from_struct_sockaddr(
               client_addr,
               &((node_tab[node_current].network).ip),
               &((node_tab[node_current].network).port)
            );

            // {//test

            //    unsigned char ip[4];
            //    char ip_str[16];

            //    ip_int2ip((node_tab[node_current].network).ip, ip);
            //    ip2srt(ip_str, ip);
            //    printf("client ip:port : %s:%hu\n", ip_str, (node_tab[node_current].network).port);

            // }

            // // Generate Virtual IP
            // attacks_get_net_ip_by_attack_id(net_ip, &mask, bootstrap_req.attack_id);
            
            // node_current++;

            // // Generate View
            //    int view_size = 0;

            // // Fill in structures
            //    struct node_network_info *node_network_info = add_node_network_info(bootstrap_req.ip,bootstrap_req.port,virtual_ip);

            //    struct node_info * node_info = add_node_info(node_network_info,view_size,bootstrap_req.attack_id);

            //    update_attacks_info(node_network_info,bootstrap_req.attack_id);

            //    print_list_attackers();
               
               


            // // Send Data to Node
            //    send_data(client_fd,(char*) &bootstrap_res,sizeof(bootstrap_res),"data to node");
               
            // Set .revents of listening socket back to default
            pollfds[i].revents = 0;
         } else if (pollfds[i].fd != listen_fd && pollfds[i].revents & POLLHUP) { // If a socket previously created to communicate with a client detects a disconnection from the client side
            
            
            // free memory
            
            // Close socket and set struct pollfd back to default
            close(pollfds[i].fd);
            pollfds[i].fd = -1;
            pollfds[i].events = 0;
            pollfds[i].revents = 0;
         }
      }
   }

/**********************************************************/

/************** Generate view and Send Data **************/



/**********************************************************/
   
/************** Memory free**************/
   
   attacks_free_tab();
   free(node_tab);



/**********************************************************/
   
   return 0;
   
}