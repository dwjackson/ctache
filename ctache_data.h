#ifndef CTACHE_DATA_H
#define CTACHE_DATA_H

enum ctache_data_type {
    CTACHE_DATA_HASH,
    CTACHE_DATA_ARRAY,
    CTACHE_DATA_STRING,
    CTACHE_DATA_NUMBER,
    CTACHE_DATA_BOOLEAN
};

typedef struct ctache_data ctache_data_t;

ctache_data_t
*ctache_data_create_array(size_t element_size, size_t num_elements);

ctache_data_t
*ctache_data_create(enum ctache_data_type data_type,
                    void *data,
                    size_t element_size,
                    size_t num_elements);

void
ctache_data_destroy(void *data);

#endif /* CTACHE_DATA_H */
