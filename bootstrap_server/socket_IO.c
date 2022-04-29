#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "socket_IO.h"

#include <poll.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>




int socket_listen_and_bind(int Nb_node,char *port){

	int listen_fd = -1;
	if (-1 == (listen_fd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	int yes = 1;
	if (-1 == setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	struct addrinfo indices;
	memset(&indices, 0, sizeof(struct addrinfo));
	indices.ai_family = AF_INET;
	indices.ai_socktype = SOCK_STREAM; //TCP
	indices.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	struct addrinfo *res, *tmp;

	int err = 0;
	if (0 != (err = getaddrinfo("0.0.0.0", port, &indices, &res))) {
		errx(1, "%s", gai_strerror(err));
	}

	tmp = res;
	while (tmp != NULL) {
		if (tmp->ai_family == AF_INET) {
			struct sockaddr_in *sockptr = (struct sockaddr_in *)(tmp->ai_addr);
			struct in_addr local_address = sockptr->sin_addr;
			#ifdef DEBUG
				printf("Binding to %s on port %hd\n",inet_ntoa(local_address),ntohs(sockptr->sin_port));
			#endif
			

			if (-1 == bind(listen_fd, tmp->ai_addr, tmp->ai_addrlen)) {
				perror("Binding");
			}
			if (-1 == listen(listen_fd, Nb_node)) {
				perror("Listen");
			}
			freeaddrinfo(res);
			return listen_fd;
		}
		tmp = tmp->ai_next;
		
	}
	freeaddrinfo(res);
	return listen_fd;
}

int socket_and_connect(char *hostname, char *port,char* ip_str){
	
	int sock_fd = -1;
	
	// Création de la socket
	if (-1 == (sock_fd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	printf("Socket created (%d)\n", sock_fd);
	struct addrinfo hints, *res, *tmp;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	int error;
	error = getaddrinfo(hostname, port, &hints, &res);
	if (error) {
		errx(1, "%s", gai_strerror(error));
		exit(EXIT_FAILURE);
	}
	tmp = res;
	while (tmp != NULL) {
		if (tmp->ai_addr->sa_family == AF_INET) {
			struct sockaddr_in *sockin_ptr = (struct sockaddr_in *)tmp->ai_addr;
			u_short port_number = sockin_ptr->sin_port;
			strcpy(ip_str,inet_ntoa(sockin_ptr->sin_addr));
			printf("Address is %s:%hu\n", ip_str, htons(port_number));
			printf("Connecting...");
			fflush(stdout);
			if (-1 == connect(sock_fd, tmp->ai_addr, tmp->ai_addrlen)) {
				perror("Connect");
				exit(EXIT_FAILURE);
			}
			printf("OK\n");
			freeaddrinfo(res);
			return sock_fd;
		}
		tmp = tmp->ai_next;
	}
	freeaddrinfo(res);
	return -1;
}

int receive_data(int sckt, char * buffer, size_t size_p, char*err_msg){

   //Receiving data
   int treated_size = 0;
   char *data_cursor = buffer;
   int err;
   int max_attempts = 10000;
   int attempts=0;

	while(treated_size < size_p){
		err = read(sckt, data_cursor, size_p-treated_size);
		if(err == -1){
			perror(err_msg);
			exit(EXIT_FAILURE);
		}
		treated_size+=err;
		data_cursor+=err;
		if(err == 0){
			attempts ++;
			if(attempts == max_attempts){
				buffer[treated_size] = '\0';
				
				return 0;

			}
		}else{
			attempts = 0;
		}
	}

   return 1;

}

void send_data(int sckt, char *buff, size_t size,char*err_msg){

	int ret = 0, offset = 0;
	int max_attempts = 10000;
	int attempts=0;

	while (offset != size) {
		if (-1 == (ret = write(sckt, buff + offset, size - offset))) {
			perror(err_msg);
			exit(EXIT_FAILURE);
		}
		if(ret == 0){
			attempts++;
			if(attempts == max_attempts){
			fprintf(stderr, "Data_send : Max attempts reached.\n");
			exit(EXIT_FAILURE);
			}
		}else{
			attempts = 0;
		}
		offset += ret;
	}
}