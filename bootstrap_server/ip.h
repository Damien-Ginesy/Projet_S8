#ifndef IP_H_
#define IP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

void ip_alloc_init(int mask1_nbraddr, int mask2_nbraddr, int mask3_nbraddr, int mask4_nbraddr);
void ip_alloc_free();

void ip_alloc_get_registerd_net_ip_by_mask(
    unsigned char **mask_allocated_adrr,
    int *mask_allocated_adrr_curser,
    char mask
);
void ip_alloc_network_ip(unsigned char *ip, char mask);
char ip_is_alloc(unsigned char *ip, char mask);
void ip_alloc(unsigned char *ip_out, unsigned char *net_ip, char mask);

void ip_int2ip(int ip_int, unsigned char *ip);
int ip_ip2int(unsigned char *ip);

// str conversion
void ip2srt(char *str_out, unsigned char *ip);
int ip_str2intip(char *str);

void ip_print(unsigned char *ip);

// other tools
void ip_and_port_from_struct_sockaddr(
    struct sockaddr sockaddr_in,
    uint32_t *ip_out,
    uint16_t *port_out
);

// Tests
void ip_test_conv();
void ip_print_after_init();
void ip_test_alloc_net_ip();
void ip_test_alloc_ip();
void ip_final_test();

#endif