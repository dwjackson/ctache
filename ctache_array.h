#ifndef CTACHE_ARRAY_H
#define CTACHE_ARRAY_H

typedef struct ctache_array ctache_array_t;

ctache_array_t
*ctache_array_create(size_t element_size, size_t num_elements);

void
ctache_array_destroy(void *data);

#endif /* CTACHE_ARRAY_H */
