#ifndef CTACHE_ARRAY_H
#define CTACHE_ARRAY_H

typedef struct ctache_array ctache_array_t;

/*
 * Create a (dynami) ctache array. The element size must be given in bytes.
 */
ctache_array_t
*ctache_array_create(size_t element_size, size_t num_elements);

void
ctache_array_destroy(void *data);

void
*ctache_array_get(ctache_array_t *array, int index);

void
ctache_array_set(ctache_array_t *array, size_t index, void *data);

#endif /* CTACHE_ARRAY_H */
