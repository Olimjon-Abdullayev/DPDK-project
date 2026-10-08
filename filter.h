#ifndef FILTER_H
#define FILTER_H
#include <rte_mbuf.h>

int in_black_list(struct rte_mbuf *m);

#endif