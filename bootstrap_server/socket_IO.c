#include "socket_IO.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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