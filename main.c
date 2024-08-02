#define _GNU_SOURCE 
#include "xallve_container/xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include <linux/sched.h>

#define STACK_SIZE (1024*1024)

int container_main(void* arg) {
    Xallve_Container* container = (Xallve_Container*)arg;

    // Setup container filesystem
    setup_filesystem(container);

    // CGroups config to limit resourses
    setup_cgroups(container->name);

    // Container network config
    setup_network(container->name, container->ip_address);

    // Processes here
    // Processes here
    run_dependencies(container);
    run_command(container);

    return 0;
}

int main() {
    // TODO add cli to manage this
    Xallve_Container* main_container = create_container("xallve_container",
                                                        "/tmp/xallve_container",
                                                        "/bin/sh",
                                                        "10.0.0.2", 
                                                        512, 
                                                        1024 * 1024 * 64, 
                                                        "apt-get update && apt-get install -y python3 python3-pip");
                                                        // 512 CPU shares, 64 MB memory limit, Python installation
    if (main_container) {
        printf("Container created: %s\n", main_container->name);

        // Stack allocation for new process
        char* stack = malloc(STACK_SIZE);
        if (!stack) {
            perror("Failed to allocate stack");
            destroy_container(main_container);
            return EXIT_FAILURE;
        }

        // Creating new process
        pid_t pid = clone(container_main, stack + STACK_SIZE, CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWNET | SIGCHLD, main_container);
        if (pid == -1) {
            perror("Failed to clone");
            free(stack);
            destroy_container(main_container);
            return EXIT_FAILURE;
        }

        // Main process
        waitpid(pid, NULL, 0);  // Wait for child process to die

        destroy_cgroups(main_container->name);

        free(stack);
        destroy_container(main_container);
    } else {
        printf("Failed to create container\n");
    }
    
    return 0;
}