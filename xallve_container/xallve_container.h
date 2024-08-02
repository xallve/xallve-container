#ifndef XALLVE_CONTAINER_H
#define XALLVE_CONTAINER_H

#define CONTAINER_MOUNT_POINT "/mnt/container"


typedef struct {
    char name[256];
    char rootfs_path[1024];
    char command[256];
    char ip_address[16];
    int cpu_shares; // CPU limit
    int memory_limit_in_bytes; // Memory limit
    char setup_script[1024];
    // Later more
} Xallve_Container;

// Container creation
Xallve_Container* create_container(const char* name,
                                   const char* rootfs_path,
                                   const char* command,
                                   const char* ip_address,
                                   int         cpu_shares,
                                   int         memory_limit_in_bytes,
                                   const char* setup_script);


void setup_filesystem(const Xallve_Container* container);
void destroy_container(Xallve_Container* container);
void run_command(const Xallve_Container* conatiner);
void run_dependencies(const Xallve_Container* container);

void setup_cgroups(const Xallve_Container* container);
void destroy_cgroups(const char *name);

void setup_network(const char *name, const char *ip_address);

void copy_file_to_container(const Xallve_Container* container, 
                            const char*             src_path, 
                            const char*             dest_path);

void copy_file_from_container(const Xallve_Container* container, 
                              const char*             src_path, 
                              const char*             dest_path);

#endif // XALLVE_CONTAINER_H