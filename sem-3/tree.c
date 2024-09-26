#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <err.h>

void print_tree(int dirfd, char* prefix, size_t prefix_len) {
    DIR* dir = fdopendir(dirfd);

    struct dirent* entry = readdir(dir);
    struct dirent* next_entry = readdir(dir);
    while (next_entry != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s|-- %s\n", prefix, entry->d_name);
    
            if (entry->d_type == DT_DIR) {
                int fd = openat(dirfd, entry->d_name, O_RDONLY, 0);
                
                strcat(prefix, "|   ");
                print_tree(fd, prefix, prefix_len + 4);
                prefix[prefix_len] = '\0';
            }
        }

        entry = next_entry;
        next_entry = readdir(dir);
    }
    if (entry != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("%s`-- %s\n", prefix, entry->d_name);

            if (entry->d_type == DT_DIR) {
                int fd = openat(dirfd, entry->d_name, O_RDONLY, 0);
                
                strcat(prefix, "    ");
                print_tree(fd, prefix, prefix_len + 4);
                prefix[prefix_len] = '\0';
            }
        }
    }
    closedir(dir);
}

int main(int argc, char** argv) {
    int dirfd = open(argv[1], O_RDONLY|O_DIRECTORY, 0);
    if (dirfd == -1) {
        err(EXIT_FAILURE, "open: argument must be a directory");
    }

    char prefix[1024];
    prefix[0] = '\0';

    printf("%s\n", argv[1]);
    print_tree(dirfd, prefix, 0);

    return 0;
}
