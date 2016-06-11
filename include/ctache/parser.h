/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef PARSER_H
#define PARSER_H

#include "linked_list.h"

struct linked_list
*ctache_parse(struct linked_list *tokens);

#endif /* PARSER_H */