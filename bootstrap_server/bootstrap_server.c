#include <stdio.h>
#include "socket_IO.h"
#include "bootstrap.h"
#include "exchange_protocol.h"
#include "attack.h"


int main(int argc, char *argv[]) {

/**** Parse simulation parametres and init db struct ******/
parse_simu_params(argc, argv);
attack_generate_net_ip();

#ifdef DEBUG
{
   printf("attacks nbr = %d\n", attacks_nbr);
   for(int i = 0; i < attacks_nbr; i++){

      char str_ip[16];
      ip2srt(str_ip, attacks_info[i].network_ip);

      printf(
         "attack : id=%d, net_ip=%s, mask=%d\n",
         attacks_info[i].id,
         str_ip,
         attacks_info[i].mask
      );
   }
}
#endif

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
   char mask;
   unsigned char ip[4];

   char still_unconnected_nodes = 1;
   while(still_unconnected_nodes){

      int n_active =0;
         // Block until new activity detected on existing socket
		if (-1 == (n_active = poll(pollfds, node_nbr+1, -1))) {
			perror("Poll");
		}


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
            
            // store new file descriptor in available slot in the array of struct pollfd set .events to POLLIN
            for (int j = 0; j < node_nbr+1; j++) {
               if (pollfds[j].fd == -1 && j != i) {
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

            // Saving node info

            node_tab[node_current].attaque_id = bootstrap_req.attack_id;
            node_tab[node_current].view_size = bootstrap_req.view_size;

            ip_and_port_from_struct_sockaddr(
               client_addr,
               &((node_tab[node_current].network).ip),
               &((node_tab[node_current].network).port)
            );


            if(bootstrap_req.attack_id != 0){ // malicious node

               // Generate Virtual IP

               attacks_get_net_ip_by_attack_id(net_ip, &mask, bootstrap_req.attack_id);

               if(mask==0){ // eclipse

                  ip_alloc(ip, NULL, 0);
               
               }else{ //inst
               
                  ip_alloc(ip, net_ip, mask);
               
               }

               node_tab[node_current].network.virtual_ip = ip_ip2int(ip);

               // register as an attacker

               attack_register_attacker(bootstrap_req.attack_id, &(node_tab[node_current].network));

            }else{ // non malicious node

               // Generate Virtual IP

               ip_alloc(ip, NULL, 0);
               node_tab[node_current].network.virtual_ip = ip_ip2int(ip);

            }

            #ifdef DEBUG
            {
               unsigned char ip[4];
               char ip_str[16];
               ip_int2ip((node_tab[node_current].network).ip, ip);
               ip2srt(ip_str, ip);

               unsigned char vip[4];
               char vip_str[16];
               ip_int2ip(node_tab[node_current].network.virtual_ip, vip);
               ip2srt(vip_str, vip);

               printf(

                  "%s:%hu\n\tattack id : %d\n\tview_size : %d\n\tvirtual ip : %s\n",
                  ip_str,
                  (node_tab[node_current].network).port,
                  node_tab[node_current].attaque_id,
                  node_tab[node_current].view_size,
                  vip_str
               );

            }
            #endif
            

            node_current++;

            if(node_current == node_nbr){
               still_unconnected_nodes = 0;
               break;
            }
               
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
   printf("Responding : \n");


/**********************************************************/
   
/************** Memory free**************/
   
   attacks_free_tab();
   free(node_tab);


/**********************************************************/
   
   return 0;
   
}