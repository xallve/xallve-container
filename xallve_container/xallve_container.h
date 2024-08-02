#ifndef XALLVE_CONTAINER_H
#define XALLVE_CONTAINER_H

#define CONTAINER_MOUNT_POINT "/mnt/container"


typedef struct {
    char name[256];
    char rootfs_path[1024];
    char command[256];
    char ip_address[16];
    // Later more
} Xallve_Container;

// Container creation
Xallve_Container* create_container(const char*        name,
                                   const char* rootfs_path,
                                   const char*     command,
                                   const char*  ip_address);


void setup_filesystem(const Xallve_Container* container);
void destroy_container(Xallve_Container* container);
void run_command(const Xallve_Container *conatiner);

void setup_cgroups(const char *name);
void destroy_cgroups(const char *name);

void setup_network(const char *name, const char *ip_address);

#endif // XALLVE_CONTAINER_H