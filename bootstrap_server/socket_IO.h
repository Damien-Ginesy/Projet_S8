#ifndef __socket_IO_H__
#define __socket_IO_H__

#include <stdlib.h>

void *receive_data(int sckt, size_t *size_p);
void send_data(int sckt, void *data, size_t size);

#endif