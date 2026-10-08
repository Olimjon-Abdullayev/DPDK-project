#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_byteorder.h>
#include <netinet/in.h>
#include "filter.h"

int in_black_list(struct rte_mbuf *m)
{
    struct rte_ether_hdr *eth;
    struct rte_ipv4_hdr *ipv4;

    eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

    // Check if the packet is IPv4
    if (rte_be_to_cpu_16(eth->ether_type) == RTE_ETHER_TYPE_IPV4) {
        
        ipv4 = (struct rte_ipv4_hdr *)(eth + 1);

        // Task 4: Block ICMP traffic (ping)
        // IPPROTO_ICMP is a standard Linux variable that equals 1
        if (ipv4->next_proto_id == IPPROTO_ICMP) {
            return 1; // Drop the ICMP packet
        }
    }

    // Return 0 to allow all other protocols (like TCP and UDP) to pass
    return 0; 
}