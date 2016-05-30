#include <stdlib.h>
#include "ctache_array.h"

#define BUF_SIZE 10

typedef char byte;

struct ctache_array {
    size_t bufsize;
    size_t length;
    size_t element_size;
    byte buffer[];
};

ctache_array_t
*ctache_array_create(size_t element_size, size_t num_elements)
{
    struct ctache_array *array;
    size_t bufsize = num_elements;
    if (num_elements < BUF_SIZE) {
        bufsize = BUF_SIZE;
    }
    array = malloc(bufsize * element_size);
    if (array != NULL) {
        array->bufsize = bufsize;
        array->length = 0;
        array->element_size = element_size;
    }
    return array;
}

void
ctache_array_destroy(void *data)
{
    struct ctache_array *array = (struct ctache_array *)(data);
    free(array);
}

void
*ctache_array_get(ctache_array_t *array, int index)
{
    void *data;
    if (index > array->length) {
        return NULL;
    }
    byte *byte_ptr = (array->buffer) + index * array->element_size;
    data = (void *)(byte_ptr);
    return data;
}

void
ctache_array_set(ctache_array_t *array, size_t index, void *data)
{
    byte *byte_ptr = (array->buffer) + index * array->element_size;
    byte *data_ptr = (byte *)(data);
    int i;
    for (i = 0; i < array->element_size; i++) {
        *byte_ptr = *data_ptr;
        byte_ptr++;
        data_ptr++;
    }
}

void
ctache_array_append(ctache_array_t **array_ptr, void *data)
{
    struct ctache_array *array = *array_ptr;
    if (array->length + 1 >= array->bufsize) {
        size_t bufsize = array->bufsize *= 2;
        *array_ptr = realloc(*array_ptr, sizeof(struct ctache_array) + bufsize);
        array = *array_ptr;
    }
    ctache_array_set(*array_ptr, array->length, data);
    ((*array_ptr)->length)++;
}

#undef BUF_SIZE
