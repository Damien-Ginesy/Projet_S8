#ifndef IP_H_
#define IP_H_

void ip_alloc_init(int mask1_nbraddr, int mask2_nbraddr, int mask3_nbraddr, int mask4_nbraddr);
void ip_alloc_free();

void ip_int2ip(int ip_int, unsigned char *ip);
int ip_ip2int(unsigned char *ip);

void ip_print(unsigned char *ip);

void ip_test();

#endif