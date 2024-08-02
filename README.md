# xallve-container
This project provides a basic containerization application written in C, inspired by Docker. It allows users to create and manage lightweight containers with resource constraints, run custom commands, and install dependencies.


## Features
* Filesystem Isolation: Each container has its own filesystem.
* Resource Limitation: Set CPU shares and memory limits for containers using cgroups.
* Custom Commands: Run user-defined commands within the container.
* Dependency Installation: Install required dependencies before running user code.
* Network Configuration: Assign IP addresses to containers.


## Getting started
### requirements
* A Linux system with root access.
* GCC (GNU Compiler Collection).
* Knowledge of basic C programming and Linux system calls.
