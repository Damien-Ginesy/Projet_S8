#ifndef __socket_IO_H__
#define __socket_IO_H__


#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>

int socket_listen_and_bind(int Nb_node,char *port);

int socket_bind(int Nb_proc, uint16_t* port);

int socket_and_connect(char *hostname, char *port,char* ip_str) ;

int receive_data(int sckt, char * buffer, size_t size_p,char*err_msg);
void send_data(int sckt, char *buff, size_t size,char*err_msg);

#endif