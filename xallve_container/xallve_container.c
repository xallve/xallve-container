#include "xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>

Xallve_Container* create_container(const char* name, const char* rootfs_path, const char *command) {
    Xallve_Container* container = malloc(sizeof(Xallve_Container));
    if (!container) return NULL;

    strncpy(container->name, name, sizeof(container->name) - 1);
    strncpy(container->rootfs_path, rootfs_path, sizeof(container->rootfs_path) - 1);
    strncpy(container->command, command, sizeof(container->command) - 1);

    // Create file system (empty dir)
    if (mkdir(rootfs_path, 0755) != 0) {
        perror("Failed to create container filesystem");
        free(container);
        return NULL;
    }

    // Create mounting point
    if (mkdir(CONTAINER_MOUNT_POINT, 0755) != 0) {
        perror("Failed to create mount point");
        free(container);
        return NULL;
    }

    return container;
}

void setup_filesystem(const Xallve_Container *container) {
    if(mount(container->rootfs_path, CONTAINER_MOUNT_POINT, NULL, MS_BIND, NULL) != 0) {
        perror("Failed to mount filesystem");
        exit(EXIT_FAILURE);
    }

    printf("Filesystem mounted: %s\n", container->rootfs_path);
}

void destroy_container(Xallve_Container* container) {
    if (container) {
        // Delete mounting point
        if (rmdir(CONTAINER_MOUNT_POINT) != 0) {
            perror("Failed to remove mount point");
        }

        // Delete filesystem
        if (rmdir(container->rootfs_path) != 0) {
            perror("Failed to remove container filesystem");
        }

        free(container);
    }
}

void run_command(const Xallve_Container *container) {
    printf("Running command in container: %s\n", container->command);
    system(container->command);
}
