#include <yaml.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include "yaml_data.h"
    
ctache_data_t
*data_from_yaml(const char *file_name)
{
    ctache_data_t *data = NULL;

    struct stat statbuf;
    if (stat(file_name, &statbuf) < 0) {
        return NULL;
    }
    off_t file_size = statbuf.st_size;
    int fd = open(file_name, O_RDONLY);
    if (fd >= 0) {
        void *region = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (region == (void *) -1) {
            fprintf(stderr, "mmap() failed, errno = %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        unsigned char *file_content = (unsigned char *)(region);

        yaml_parser_t parser;
        yaml_event_t event;
        int done = 0;
        yaml_parser_initialize(&parser);
        yaml_parser_set_input_string(&parser, file_content, file_size);
        yaml_char_t *key = NULL;
        yaml_char_t *value = NULL;
        while (!done) {
            if (!yaml_parser_parse(&parser, &event)) {
                goto end;
            }

            // TODO: Process the event
            switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                if (data == NULL) {
                    data = ctache_data_create_hash();
                } else {
                    abort(); // TODO
                }
                key = NULL;
                break;
            case YAML_SCALAR_EVENT:
                if (key == NULL) {
                    key = event.data.scalar.value; // TODO
                } else {
                    value = event.data.scalar.value;
                }
                if (key != NULL && value != NULL) {
                    printf("[DEBUG] %s: \"%s\"\n", key, value);
                    // TODO
                    key = NULL;
                    value = NULL;
                }
                break;
            case YAML_MAPPING_END_EVENT:
                printf("[DEBUG] mapping end\n");
                // TODO
                break;
            default:
                /* Do nothing */
                break;
            }

            done = (event.type == YAML_STREAM_END_EVENT);
            yaml_event_delete(&event);
        }
end:
        yaml_parser_delete(&parser);
        munmap(region, file_size);
        close(fd);
    }

    return data;
}
