#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include "ctache_data.h"
#include "escaping.h"

#define CTACHE_RENDER_FLAG_PRINT_TOKENS 0x1
#define CTACHE_RENDER_FLAG_PRINT_RULES  0x2

void
ctache_render_file(FILE *in_fp,
                   FILE *out_fp,
                   ctache_data_t *data,
                   int flags,
                   enum escaping_type escaping_type);

#endif /* RENDER_H */
