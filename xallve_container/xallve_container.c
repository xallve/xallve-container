#include "xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>

Xallve_Container* create_container(const char* name,
                                   const char* rootfs_path, 
                                   const char* command, 
                                   const char* ip_address,
                                   int         cpu_shares,
                                   int         memory_limit_in_bytes,
                                   const char* setup_script) {
    Xallve_Container* container = malloc(sizeof(Xallve_Container));
    if (!container) return NULL;

    strncpy(container->name, name, sizeof(container->name) - 1);
    strncpy(container->rootfs_path, rootfs_path, sizeof(container->rootfs_path) - 1);
    strncpy(container->command, command, sizeof(container->command) - 1);
    strncpy(container->ip_address, ip_address, sizeof(container->ip_address) - 1);
    container->cpu_shares = cpu_shares;
    container->memory_limit_in_bytes = memory_limit_in_bytes;
    strncpy(container->setup_script, setup_script, sizeof(container->setup_script) - 1);

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
        if (umount(CONTAINER_MOUNT_POINT) != 0) {
            perror("Failed to unmont filesystem");
        }
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

void run_dependencies(const Xallve_Container* container) {
    printf("Installing dependencies in container: %s\n", container->setup_script);
    system(container->setup_script);
}

void copy_file_to_container(const Xallve_Container* container, const char* src_path, const char* dest_path) {
    char dest_full_path[1024];
    snprintf(dest_full_path, sizeof(dest_full_path), "%s/%s", CONTAINER_MOUNT_POINT, dest_path);

    char command[2048];
    snprintf(command, sizeof(command), "cp %s %s", src_path, dest_full_path);
    if (system(command) != 0) {
        perror("Failed to copy file to container");
    }
}

void copy_file_from_container(const Xallve_Container* container, const char* src_path, const char* dest_path) {
    char src_full_path[1024];
    snprintf(src_full_path, sizeof(src_full_path), "%s/%s", CONTAINER_MOUNT_POINT, src_path);

    char command[2048];
    snprintf(command, sizeof(command), "cp %s %s", src_full_path, dest_path);
    if (system(command) != 0) {
        perror("Failed to copy file from container");
    }
}