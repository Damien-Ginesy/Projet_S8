#include <stdio.h>
#include "socket_IO.h"
#include "bootstrap.h"
#include "exchange_protocol.h"



int main(int argc, char *argv[]) {

/****************** Listening socket ************************/
// Create listening socket
	char *port = argv[1];
	int listen_fd = -1;
	if (-1 == (listen_fd = socket_listen_and_bind(NODE_NUM,port))) {
		printf("Could not create, bind and listen properly\n");
		return 1;
	}


/**********************************************************/

/*********** Data read and update and generate IP *********/



	// Handle new connections and existing ones
	
	// Declare array of struct pollfd
	struct pollfd pollfds[NODE_NUM+1];

	// Init first slot with listening socket
	pollfds[0].fd = listen_fd;
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	// Init remaining slot to default values
	for (int i = 1; i < NODE_NUM+1; i++) {
		pollfds[i].fd = -1;
		pollfds[i].events = 0;
		pollfds[i].revents = 0;
	}

   struct bootstrap_req bootstrap_req;
   struct bootstrap_res bootstrap_res;

   while(1){
      printf("pass la \n");
      int n_active =0;
         // Block until new activity detected on existing socket
		if (-1 == (n_active = poll(pollfds, NODE_NUM+1, -1))) {
			perror("Poll");
		}
		printf("[SERVER] : %d active socket\n", n_active);

         
      for (int i = 0; i < NODE_NUM+1; i++){
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
            for (int j = 0; j < NODE_NUM+1; j++) {
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
            // Generate Virtual IP


            // Generate View


            // Send Data to Node
               send_data(client_fd,(char*) &bootstrap_res,sizeof(bootstrap_res),"data to node");
               
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
    return 0;
   
}