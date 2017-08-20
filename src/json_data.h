/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * Copyright (c) 2016 David Jackson
 */

#ifndef JSON_DATA_H
#define JSON_DATA_H

#include "ctache/ctache.h"

ctache_data_t
*data_from_json(const char *file_name);

#endif /* JSON_DATA_H */
