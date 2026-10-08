#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_byteorder.h>
#include "filter.h"

int in_black_list(struct rte_mbuf *m)
{
    struct rte_ether_hdr *eth;
    struct rte_ipv4_hdr *ipv4;
    uint32_t src_ip, dst_ip;

    // Define the Target Subnet (192.168.1.0) and Mask (/28 = 255.255.255.0)
    uint32_t target_subnet = RTE_IPV4(192, 168, 1, 0);
    uint32_t subnet_mask   = RTE_IPV4(255, 255, 255, 0);

    eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

    if (rte_be_to_cpu_16(eth->ether_type) == RTE_ETHER_TYPE_IPV4) {
        
        ipv4 = (struct rte_ipv4_hdr *)(eth + 1);

        src_ip = rte_be_to_cpu_32(ipv4->src_addr);
        dst_ip = rte_be_to_cpu_32(ipv4->dst_addr);

        // Apply the mask and check if it matches the target subnet
        if ((src_ip & subnet_mask) == target_subnet || 
            (dst_ip & subnet_mask) == target_subnet) {
            return 1; // Drop packet
        }
    }

    return 0; 
}