#include <stdio.h>
#include "socket_IO.h"
#include "bootstrap.h"


int main(int argc, char *argv[]) {

/****************** Listen socket ************************/



/**********************************************************/

/*********** Data read and update and generate IP *********/

const int poll_timeout = 500;       // micro secondes
   const int receiving_timeout = 4;   // multiple of poll_timeout
   int receiving_time_counter = 0;     // multiple of poll_timeout

   int active_procs_but_me = NODE_NUM-1;

   struct pollfd pollfds[NODE_NUM-1];
   int i = 0;
  for (int j=0; j < NODE_NUM; j++){
      
      if(j != NODE_ID){
         
         pollfds[i].fd = proc_conn_info[j].fd;
         pollfds[i].events = POLLIN;
         pollfds[i].revents = 0;
         i++;
      }
   }

   dsm_msg_header_t header_msg_recv;
   dsm_msg_header_t header_msg_send;
   dsm_req_type_t page_info;const int poll_timeout = 500;       // micro secondes
   const int receiving_timeout = 4;   // multiple of poll_timeout
   int receiving_time_counter = 0;     // multiple of poll_timeout

   int active_procs_but_me = NODE_NUM-1;

   struct pollfd pollfds[NODE_NUM-1];
   int i = 0;
  for (int j=0; j < NODE_NUM; j++){
      
      if(j != NODE_ID){
         
         pollfds[i].fd = proc_conn_info[j].fd;
         pollfds[i].events = POLLIN;
         pollfds[i].revents = 0;
         i++;
      }
   }

   dsm_msg_header_t header_msg_recv;
   dsm_msg_header_t header_msg_send;
   dsm_req_type_t page_info;

while(1){

      if (-1 == poll(pollfds, NODE_NUM-1, poll_timeout)) {
			perror("Poll");
		}

      // Verifying receiving_timeout
      pthread_mutex_lock(&requested_page_mutex);
      receiving_time_counter++;
      if(requested_page >= 0 && receiving_time_counter == receiving_timeout){

         /*Try again*/
         receiving_time_counter = 0;
         pthread_mutex_unlock(&available_page);
      }
      pthread_mutex_unlock(&requested_page_mutex);

      // Daemon exit condition
      pthread_mutex_lock(&finalize_mutex);
      if(finalize && active_procs_but_me <= 0){
         return NULL;
      }
      pthread_mutex_unlock(&finalize_mutex);
      
      for (int j = 0; j < NODE_NUM-1; j++){


         if (pollfds[j].fd != -1 && pollfds[j].revents & POLLIN){

            /* réception du header */
            if(! dsm_recv(pollfds[j].fd,(char*) &header_msg_recv,sizeof(dsm_msg_header_t),"header_recv")){
               pollfds[j].revents = 0;
               continue;
            }

            switch (header_msg_recv.req_type){

               case DSM_REQ:

                  /*Verify if I am the owner of the page*/
                  if(table_page[header_msg_send.page_num].owner == NODE_ID){

                     /* envoi du header */
                     header_msg_send.page_num = header_msg_recv.page_num; // pour l'envoyer au deamon
                     header_msg_send.req_type = DSM_PAGE;
                     header_msg_send.taille = PAGE_SIZE;
                     dsm_send(pollfds[j].fd,(char*) &header_msg_send,sizeof(dsm_msg_header_t),"header_send");

                     /* envoi des informations de la page*/
                     dsm_send(pollfds[j].fd,(char*) &table_page[header_msg_recv.page_num],sizeof(dsm_page_info_t),"page_info");

                     /* envoi de la page*/
                     dsm_send(pollfds[j].fd,(char*)num2address(header_msg_send.page_num), PAGE_SIZE, "page_send");

                     /* Update table_page for me */
                     table_page[header_msg_send.page_num].owner = proc_conn_info[conn_info_get_index_by_fd(pollfds[j].fd)].rank;
                     
                     /* libérer la page */
                     dsm_free_page(header_msg_send.page_num);

                     /* Sending page info to all procs but me & the receiver */

                     header_msg_send.req_type = DSM_UPDATE;
                     header_msg_send.taille = 0;

                     for(int i = 0; i<NODE_NUM; i++){
                        if(proc_conn_info[i].rank != NODE_ID && proc_conn_info[i].rank != pollfds[j].fd){
                           dsm_send(proc_conn_info[i].fd, (char*) &header_msg_send, sizeof(dsm_msg_header_t), "update_page_status_header_send");
                           dsm_send(proc_conn_info[i].fd, (char*) &table_page[header_msg_send.page_num], sizeof(dsm_page_info_t), "update_page_status_send");
                        }
                     }

                  }

                  break;

               case DSM_PAGE:

                  /* réception des informations de la page*/
                  dsm_recv(pollfds[j].fd, (char*) &table_page[header_msg_recv.page_num], sizeof(dsm_page_info_t),"info_page");
                  
                  /* mis à jour des informations de la page*/
                  table_page[header_msg_recv.page_num].owner = NODE_ID;

                  /* Allocation de la page */
                  dsm_alloc_page(header_msg_recv.page_num);

                  /* réception de la page */
                  dsm_recv(pollfds[j].fd,(char*)num2address(header_msg_recv.page_num),PAGE_SIZE,"page_recv");

                  pthread_mutex_lock(&requested_page_mutex);

                  if(requested_page == header_msg_recv.page_num){
                     /*signaler la disponibilité de la page*/
                     receiving_time_counter = 0;
                     pthread_mutex_unlock(&available_page);
                  }else{
                     //fprintf(stderr, "I am receiving an other page instead of the requested one.\n");
                     break;
                     //exit(EXIT_FAILURE);
                  }

                  pthread_mutex_unlock(&requested_page_mutex);
                  

                  break;

               case DSM_UPDATE:

                  /* I'am obliged to read the message otherwise the sender will be blocked*/
                  dsm_recv(pollfds[j].fd, (char*) & page_info, sizeof(dsm_page_info_t), "update_info_page_receive");

                  /*
                     When it comes to my pages, I'm always right.
                     So I ignore the update message.
                  */
                  if(table_page[header_msg_recv.page_num].owner != NODE_ID){

                     memcpy(&table_page[header_msg_recv.page_num], &page_info, sizeof(dsm_page_info_t));

                     /*
                        Give an other chance to the main thread (re-request the page after a new seg fault)
                        if it hasn't requested the needed page from its right owner.
                     */
                     pthread_mutex_lock(&requested_page_mutex);

                     if(requested_page == header_msg_recv.page_num){
                        receiving_time_counter = 0;
                        pthread_mutex_unlock(&available_page);
                     }

                     pthread_mutex_unlock(&requested_page_mutex);
                  }

                  break;
               
               case DSM_FINALIZE:
                  active_procs_but_me--;
                  break;

               default:
                  break;
            }

         }else if(pollfds[j].revents !=0){
            pollfds[j].fd = -1;
            pollfds[j].events = 0;
            pollfds[j].revents = 0;
         }

         pollfds[j].revents = 0;

      }

   }


/**********************************************************/

/************** Generate view and Send Data **************/



/**********************************************************/
    return 0;
}