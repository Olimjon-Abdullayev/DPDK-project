#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_byteorder.h>
#include "filter.h"

int in_black_list(struct rte_mbuf *m)
{
    struct rte_ether_hdr *eth;
    struct rte_ipv4_hdr *ipv4;
    uint32_t src_ip, dst_ip;
    
    // Define the client IP to block
    uint32_t blocked_ip = RTE_IPV4(192, 168, 1, 50);

    eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

    // Check if the packet is an IPv4 packet
    if (rte_be_to_cpu_16(eth->ether_type) == RTE_ETHER_TYPE_IPV4) {
        
        ipv4 = (struct rte_ipv4_hdr *)(eth + 1);

        // Read both Source and Destination IPs
        src_ip = rte_be_to_cpu_32(ipv4->src_addr);
        dst_ip = rte_be_to_cpu_32(ipv4->dst_addr);

        // Drop the packet if the blocked IP is either sending OR receiving it
        if (src_ip == blocked_ip || dst_ip == blocked_ip) {
            return 1; 
        }
    }

    return 0; 
}