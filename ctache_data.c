#include <stdlib.h>
#include <stdbool.h>
#include "hash_table.h"

enum ctache_data_type {
    CTACHE_DATA_HASH,
    CTACHE_DATA_ARRAY,
    CTACHE_DATA_STRING,
    CTACHE_DATA_NUMBER,
    CTACHE_DATA_BOOL
};

struct ctache_data {
    enum ctache_data_type data_type;
    union {
        struct ctache_hash_table *hash;
        double number;
        bool boolean;
    } data;
};

struct ctache_data
*ctache_data_create(enum ctache_data_type data_type, void *data)
{
    struct ctache_hash_table *tbl;
    double *dp;
    bool *bp;

    struct ctache_data *ctache_data = malloc(sizeof(struct ctache_data));
    if (ctache_data) {
        ctache_data->data_type = data_type;
        switch (data_type) {
        case CTACHE_DATA_HASH:
            tbl = (struct ctache_hash_table *)(data);
            ctache_data->data.hash = tbl;
            break;
        case CTACHE_DATA_ARRAY:
            // TODO
            break;
        case CTACHE_DATA_STRING:
            // TODO
            break;
        case CTACHE_DATA_NUMBER:
            dp = (double *)(data);
            ctache_data->data.number = *dp;
            break;
        case CTACHE_DATA_BOOL:
            bp = (bool *)(data);
            ctache_data->data.boolean = *bp;
            break;
        default:
            // TODO
            break;
        }
    }
    return ctache_data;
}

void
ctache_data_destroy(void *data)
{
    struct ctache_data *ctache_data = (struct ctache_data *)(data);
    switch (ctache_data->data_type) { // TODO
    default:
        break;
    }
    // TODO
}
