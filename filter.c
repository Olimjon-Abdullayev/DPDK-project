#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_byteorder.h>
#include <netinet/in.h>
#include "filter.h"

int in_black_list(struct rte_mbuf *m)
{
    struct rte_ether_hdr *eth;
    struct rte_ipv4_hdr *ipv4;
    struct rte_tcp_hdr *tcp;

    eth = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);

    // 1. Check if the packet is IPv4
    if (rte_be_to_cpu_16(eth->ether_type) == RTE_ETHER_TYPE_IPV4) {
        
        ipv4 = (struct rte_ipv4_hdr *)(eth + 1);

        // 2. Check if the protocol is TCP
        if (ipv4->next_proto_id == IPPROTO_TCP) {
            
            // 3. The TCP header is located right after the IPv4 header
            tcp = (struct rte_tcp_hdr *)(ipv4 + 1);

            // 4. Task 6: Block if the destination port is 80 (HTTP)
            if (rte_be_to_cpu_16(tcp->dst_port) == 80) {
                return 1; // Drop the HTTP packet
            }
        }
    }

    // Return 0 to allow everything else (like ping, UDP, or other TCP ports)
    return 0; 
}