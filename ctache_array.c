#include <stdlib.h>
#include "ctache_array.h"

#define BUF_SIZE 10

typedef char byte;

struct ctache_array {
    int bufsize;
    int length;
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
    }
    return array;
}

void
ctache_array_destroy(void *data)
{
    struct ctache_array *array = (struct ctache_array *)(data);
    free(array);
}

#undef BUF_SIZE
