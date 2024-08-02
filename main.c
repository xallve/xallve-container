#include "xallve_container/xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include <linux/sched.h>

int container_main(void* arg) {
    Xallve_Container* container = (Xallve_Container*)arg;

    // Setup container filesystem
    setup_filesystem(container);

    // Processes here
    // Processes here

    return 0;
}

int main() {
    Xallve_Container* main_container = create_container("xallve_container", "/tmp/xallve_container");
    if (main_container) {
        printf("Container created: %s\n", main_container->name);

        // Create new process with file system isolation
        pid_t pid = fork();
        if (pid == 0) {
            // Child 
            if (unshare(CLONE_NEWNS) != 0) {
                perror("Failed to unshare namespace");
                exit(EXIT_FAILURE);
            }

            if (chdir(CONTAINER_MOUNT_POINT) != 0) {
                perror("Failed to change directory");
                exit(EXIT_FAILURE);
            }

            return container_main(main_container);
        } else if (pid > 0) {
            // Main process
            wait(NULL); // Wait for child to die.. :(

            destroy_container(main_container);
        } else {
            perror("Failed to fork");
            destroy_container(main_container);
        }
    } else {
        printf("Failed to create container\n");
    }
    
    return 0;
}