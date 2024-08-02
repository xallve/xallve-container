#include "xallve_container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


// Not sure if this needed
void setup_network(const char* name, const char* ip_address) {
    // Create virtual interface
    char command[256];
    snprintf(command, sizeof(command), "ip link add veth0 type veth peer name %s-veth", name);
    if (system(command) != 0) {
        perror("Failed to create virtual ethernet interface");
        return;
    }

    // Bridge config
    snprintf(command, sizeof(command), "ip link set %s-veth master br0", name);
    if (system(command) != 0) {
        perror("Failed to attach veth to bridge");
        return;
    }

    // IP addr config
    snprintf(command, sizeof(command), "ip addr add %s/24 dev %s-veth", ip_address, name);
    if (system(command) != 0) {
        perror("Failed to set IP address");
        return;
    }


    snprintf(command, sizeof(command), "ip link set %s-veth up", name);
    if (system(command) != 0) {
        perror("Failed to bring up veth");
    }
}