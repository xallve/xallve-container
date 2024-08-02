#include "xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void setup_cgroups(const Xallve_Container* container) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/fs/cgroup/cpu/%s", container->name);
    mkdir(path, 0755);

    snprintf(path, sizeof(path), "/sys/fs/cgroup/memory/%s", container->name);
    mkdir(path, 0755);

    // CPU limit
    snprintf(path, sizeof(path), "/sys/fs/cgroup/cpu/%s/cpu.shares", container->name);
    FILE* cpu_file = fopen(path, "w");
    if (cpu_file) {
        fprintf(cpu_file, "%d", container->cpu_shares);
        fclose(cpu_file);
    } else {
        perror("Failed to set CPU shares");
    }

    // Memory limit
    snprintf(path, sizeof(path), "/sys/fs/cgroup/memory/%s/memory.limit_in_bytes", container->name);
    FILE* mem_file = fopen(path, "w");
    if (mem_file) {
        fprintf(mem_file, "%d", container->memory_limit_in_bytes);
        fclose(mem_file);
    } else {
        perror("Failed to set memory limit");
    }

    // Current process to CGroups
    snprintf(path, sizeof(path), "/sys/fs/cgroup/cpu/%s/tasks", container->name);
    FILE* tasks_file = fopen(path, "w");
    if (tasks_file) {
        fprintf(tasks_file, "%d", getpid());
        fclose(tasks_file);
    } else {
        perror("Failed to add process to CPU cgroup");
    }

    snprintf(path, sizeof(path), "/sys/fs/cgroup/memory/%s/tasks", container->name);
    tasks_file = fopen(path, "w");
    if (tasks_file) {
        fprintf(tasks_file, "%d", getpid());
        fclose(tasks_file);
    } else {
        perror("Failed to add process to memory cgroup");
    }
}

void destroy_cgroups(const char *name) {
    char path[256];
    snprintf(path, sizeof(path), "/sys/fs/cgroup/cpu/%s", name);
    rmdir(path);

    snprintf(path, sizeof(path), "/sys/fs/cgroup/memory/%s", name);
    rmdir(path);
}