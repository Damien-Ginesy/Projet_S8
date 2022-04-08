
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

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>




int socket_listen_and_bind(int Nb_proc, ushort* port) {
	int listen_fd = -1;
	if (-1 == (listen_fd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}
	printf("Listen socket descriptor %d\n", listen_fd);

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
	if (0 != (err = getaddrinfo("0.0.0.0",NULL, &indices, &res))) {
		errx(1, "%s", gai_strerror(err));
	}

	tmp = res;
	while (tmp != NULL) {
		if (tmp->ai_family == AF_INET) {
			
			if (-1 == bind(listen_fd, tmp->ai_addr, tmp->ai_addrlen)) {
				perror("Binding");
			}
			if (-1 == listen(listen_fd, Nb_proc)) {
				perror("Listen");
			}
            struct sockaddr_in sin;
            socklen_t len = sizeof(sin);
            if (getsockname(listen_fd, (struct sockaddr *)&sin, &len) == -1)
                perror("getsockname");

			*port = ntohs(sin.sin_port);
			freeaddrinfo(res);
			return listen_fd;
		}
		tmp = tmp->ai_next;
		
	}
	freeaddrinfo(res);
	return listen_fd;
}

int socket_and_connect(char *hostname, char *port) {
	int sock_fd = -1;
	// Création de la socket
	if (-1 == (sock_fd = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}
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

			char try_again = 1;
			while (try_again){
				try_again = 0;
				if (-1 == connect(sock_fd, tmp->ai_addr, tmp->ai_addrlen)) {
					try_again = 1;
				}
			}
			freeaddrinfo(res);
			return sock_fd;
		}
		tmp = tmp->ai_next;
	}
	freeaddrinfo(res);
	return -1;
}

void *receive_data(int sckt, size_t *size_p){
    //Receiving size
    const unsigned int max_read_tentative = 10;
    unsigned int tentative_nbr = 0;
    char err_was_0 = 1;

    int err;
    size_t treated_size = 0;
    char *data_cursor = (char *)size_p;
    while(treated_size < sizeof(size_t) && tentative_nbr < max_read_tentative){
        err = read(sckt, data_cursor, sizeof(size_t)-treated_size);
        if(err == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(err == 0){
            if(err_was_0){
                tentative_nbr++;
            }else{
                err_was_0 = 1;
            }
        }else{
            treated_size+=err;
            data_cursor+=err;

            err_was_0 = 0;
        }
    }
    if(treated_size < sizeof(size_t)){
        *size_p = 0;
        return NULL;
    }

    // Memory allocation
    void *data = malloc(*size_p);
    if(data == NULL){
        perror("read");
        exit(EXIT_FAILURE);
    }

    //Receiving data
    treated_size = 0;
    data_cursor = (char *) data;
    while(treated_size < *size_p){
        err = read(sckt, data_cursor, *size_p-treated_size);
        if(err == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        treated_size+=err;
        data_cursor+=err;
    }

    return data;
}

void send_data(int sckt, void *data, size_t size){
    //Sending size
    int err;
    size_t treated_size = 0;
    char *data_cursor = (char *) &size;
    while(treated_size < sizeof(size_t)){
        err = write(sckt, data_cursor, sizeof(size_t)-treated_size);
        if(err == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        treated_size+=err;
        data_cursor+=err;
    }

    //Sending data
    treated_size = 0;
    data_cursor = (char *) data;
    while(treated_size < size){
        err = write(sckt, data_cursor, size-treated_size);
        if(err == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        treated_size+=err;
        data_cursor+=err;
    }
}