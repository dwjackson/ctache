/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdlib.h>
#include <yaml.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdbool.h>
#include "ctache.h"
#include "yaml_data.h"

static bool
is_whitespace(unsigned char ch)
{
    return (ch == ' ' || ch == '\n' || ch == '\r');
}

static yaml_char_t
*yaml_strtrim(yaml_char_t *str, size_t length)
{
    yaml_char_t *trimmed_str = NULL;
    int starting_whitespace = 0;
    int ending_whitespace = 0;

    int i;
    unsigned char ch = '\0';
    for (i = 0; i < length; i++) {
        ch = str[i];
        if (is_whitespace(ch)) {
            starting_whitespace++;
        } else {
            break;
        }
    }

    for (i = length - 1; i >= 0; i--) {
        ch = str[i];
        if (is_whitespace(ch)) {
            ending_whitespace++;
        } else {
            break;
        }
    }

    int trimmed_length = length - starting_whitespace - ending_whitespace;
    if (trimmed_length > 0) {
        size_t size = trimmed_length * sizeof(yaml_char_t) + 1;
        trimmed_str = malloc(size);
        memset(trimmed_str, 0, size);
        memcpy(trimmed_str, str + starting_whitespace, trimmed_length);
    }
    return trimmed_str;
}

static yaml_char_t
*yaml_strdup(yaml_char_t *src, size_t length, bool do_trim)
{
    if (!do_trim) {
        size_t size = length * sizeof(yaml_char_t) + 1;
        yaml_char_t *dest_str = malloc(size);
        memset(dest_str, 0, size);
        memcpy(dest_str, src, length); 
        return dest_str;
    } else {
        return yaml_strtrim(src, length);
    }
}
    
ctache_data_t
*data_from_yaml(const char *file_name)
{
    ctache_data_t *data = NULL;
    ctache_data_t *str_data = NULL;
    yaml_parser_t parser;
    yaml_event_t event;
    int done;
    void *region;
    unsigned char *file_content;
    yaml_char_t *key;
    yaml_char_t *value;
    size_t value_len;
    struct linked_list *data_stack;
    ctache_data_t *child_data;

    struct stat statbuf;
    if (stat(file_name, &statbuf) < 0) {
        return NULL;
    }
    off_t file_size = statbuf.st_size;
    int fd = open(file_name, O_RDONLY);
    if (fd >= 0) {
        region = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (region == (void *) -1) {
            fprintf(stderr, "mmap() failed, errno = %s\n", strerror(errno));
            abort();
        }
        file_content = (unsigned char *)region;

        data_stack = linked_list_create();
        done = 0;
        yaml_parser_initialize(&parser);
        yaml_parser_set_input_string(&parser, file_content, file_size);
        key = NULL;
        value = NULL;
        while (!done) {
            if (!yaml_parser_parse(&parser, &event)) {
                goto end;
            }
            
            /* Process the event */
            switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                if (data == NULL) {
                    data = ctache_data_create_hash();
                } else {
                    linked_list_push(data_stack, data);
                    child_data = ctache_data_create_hash();
                    if (data->data_type == CTACHE_DATA_HASH) {
                        if (key == NULL) {
                            fprintf(stderr, "No key for hash\n");
                            abort();
                        }
                        char *key_str = (char*)key;
                        ctache_data_hash_table_set(data, key_str, child_data);
                    } else if (data->data_type == CTACHE_DATA_ARRAY) {
                        ctache_data_array_append(data, child_data);
                    } else {
                        fprintf(stderr, "Invalid data type\n");
                        abort();
                    }
                    data = child_data;
                }
                key = NULL;
                break;
            case YAML_SCALAR_EVENT:
                if (data->data_type == CTACHE_DATA_HASH) {
                    if (key == NULL) {
                        key = yaml_strdup(event.data.scalar.value,
                                          event.data.scalar.length,
                                          false);
                    } else {
                        value = yaml_strdup(event.data.scalar.value,
                                            event.data.scalar.length,
                                            false);
                        value_len = event.data.scalar.length;
                    }
                    if (key != NULL && value != NULL) {
                        str_data = ctache_data_create_string((char*)value, 
                                                             value_len);
                        ctache_data_hash_table_set(data, (char *)key, str_data);
                        free(key);
                        key = NULL;
                        value = NULL;
                        value_len = 0;
                        str_data = NULL;
                    }
                } else if (data->data_type == CTACHE_DATA_ARRAY) {
                    value = yaml_strdup(event.data.scalar.value,
                                        event.data.scalar.length,
                                        true);
                    if (value != NULL) {
                        value_len = event.data.scalar.length;
                        str_data = ctache_data_create_string((char*)value,
                                                             value_len);
                        ctache_data_array_append(data, str_data);
                        value_len = 0;
                        str_data = NULL;
                        value = NULL;
                    }
                } else {
                    fprintf(stderr, "Unexpected data type\n");
                    abort();
                }
                break;
            case YAML_MAPPING_END_EVENT:
                if (data_stack->length > 0) {
                    data = linked_list_pop(data_stack);
                }
                key = NULL;
                break;
            case YAML_SEQUENCE_START_EVENT:
                if (key == NULL) {
                    fprintf(stderr, "Error: No key for array\n");
                    abort();
                }
                child_data = ctache_data_create_array(sizeof(ctache_data_t), 0);
                ctache_data_hash_table_set(data, (char*)key, child_data);
                linked_list_push(data_stack, data);
                data = child_data;
                break;
            case YAML_SEQUENCE_END_EVENT:
                if (data_stack->length > 0) {
                    data = linked_list_pop(data_stack);
                }
                key = NULL;
                break;
            default:
                /* Do nothing */
                break;
            }

            done = (event.type == YAML_STREAM_END_EVENT);
            yaml_event_delete(&event);
        }
end:
        linked_list_destroy(data_stack);
        yaml_parser_delete(&parser);
        munmap(region, file_size);
        close(fd);
    }

    return data;
}
