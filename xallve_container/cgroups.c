#include "xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void setup_cgroups(const char *name) {
    char path[256];
    snprintf(path, sizeof(path), "sys/fs/cgroup/pids/%s", name);

    // Creating CGroup catalog
    if (mkdir(path, 0755) != 0) {
        perror("Failed to create cgroup directory");
        return;
    }

    // Limit number of processes
    char max_pids[32];
    snprintf(max_pids, sizeof(max_pids), "%d", 10);
    FILE *fp = fopen(strcat(path, "/pids.max"), "w");
    if (fp) {
        fputs(max_pids, fp);
        fclose(fp);
    }

    // Adding current process to CGroup
    fp = fopen(strcat(path, "/cgroup.procs"), "w");
    if (fp) {
        fprintf(fp, "%d", getpid());
        fclose(fp);
    }
}

void destroy_cgroups(const char *name) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/fs/cgroup/pids/%s", name);

    // remove CGroup catalog
    if (rmdir(path) != 0) {
        perror("Failed to remove cgroup directory");
    }
}