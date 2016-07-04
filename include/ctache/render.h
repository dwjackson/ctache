/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#ifndef CTACHE_RENDER_H
#define CTACHE_RENDER_H

#include <stdio.h>
#include "ctache_data.h"
#include "escaping.h"

#define CTACHE_RENDER_FLAG_PRINT_TOKENS 0x1
#define CTACHE_RENDER_FLAG_PRINT_RULES  0x2

void
ctache_render_string(const char *in_str,
                     size_t in_str_len,
                     FILE *out_fp,
                     ctache_data_t *data,
                     enum escaping_type escaping_type);

void
ctache_render_file(FILE *in_fp,
                   FILE *out_fp,
                   ctache_data_t *data,
                   enum escaping_type escaping_type);

#endif /* CTACHE_RENDER_H */
