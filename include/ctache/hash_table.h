#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_list.h"

struct ctache_hash_table {
    int bufsize; /* Total size of the "cells" buffer */
    int length; /* Number of elements */
    struct linked_list *cells[];
};

#endif /* HASH_TABLE */
