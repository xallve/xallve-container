#ifndef XALLVE_CONTAINER_H
#define XALLVE_CONTAINER_H

#define CONTAINER_MOUNT_POINT "/mnt/container"


typedef struct {
    char name[256];
    char rootfs_path[1024];
} Xallve_Container;

Xallve_Container* create_container(const char* name, const char* rootfs_path);
void setup_filesystem(const Xallve_Container* container);
void destroy_container(Xallve_Container* container);

#endif // XALLVE_CONTAINER_H