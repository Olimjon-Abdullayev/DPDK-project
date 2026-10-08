#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_byteorder.h>
#include "filter.h"

#define MAX_IPS 100
uint32_t blacklisted_ips[MAX_IPS];
int num_ips = 0;

void load_blacklist(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Notice: No %s found. Blacklist is empty.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0; // Remove newlines
        if (strlen(line) > 0) {
            struct in_addr addr;
            if (inet_pton(AF_INET, line, &addr) == 1) {
                blacklisted_ips[num_ips] = rte_be_to_cpu_32(addr.s_addr);
                printf("Loaded blocked IP from file: %s\n", line);
                num_ips++;
            }
        }
    }
    fclose(fp);
}

int in_black_list(struct rte_mbuf *m) {
    struct rte_ether_hdr *eth;
    struct rte_ipv4_hdr *ipv4;
    uint32_t src_ip, dst_ip;

    eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

    if (rte_be_to_cpu_16(eth->ether_type) == RTE_ETHER_TYPE_IPV4) {
        ipv4 = (struct rte_ipv4_hdr *)(eth + 1);
        src_ip = rte_be_to_cpu_32(ipv4->src_addr);
        dst_ip = rte_be_to_cpu_32(ipv4->dst_addr);

        // Loop through all IPs loaded from the text file
        for (int i = 0; i < num_ips; i++) {
            if (src_ip == blacklisted_ips[i] || dst_ip == blacklisted_ips[i]) {
                return 1; 
            }
        }
    }
    return 0; 
}