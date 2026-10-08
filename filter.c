#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_byteorder.h>
#include "filter.h"

int in_black_list(struct rte_mbuf *m)
{
    struct rte_ether_hdr *eth;
    struct rte_ipv4_hdr *ipv4;
    uint32_t src_ip;
    
    // Define the IP we want to block: 192.168.1.10
    uint32_t blocked_ip = RTE_IPV4(192, 168, 1, 50);

    // Get the Ethernet header from the packet
    eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

    // Check if the packet is IPv4 (EtherType 0x0800)
    if (rte_be_to_cpu_16(eth->ether_type) == RTE_ETHER_TYPE_IPV4) {
        
        // The IPv4 header is located immediately after the Ethernet header
        ipv4 = (struct rte_ipv4_hdr *)(eth + 1);

        // Convert the source IP from network byte order to standard CPU format
        src_ip = rte_be_to_cpu_32(ipv4->src_addr);

        // If the source IP matches our blocked IP, return 1 to drop it
        if (src_ip == blocked_ip) {
            return 1; 
        }
    }

    // Return 0 to allow all other packets to pass
    return 0; 
}