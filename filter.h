#ifndef FILTER_H
#define FILTER_H

#include <rte_mbuf.h>

void load_blacklist(const char *filename);
int in_black_list(struct rte_mbuf *m);

#endif