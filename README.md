# Localhost Traffic Monitor

This project focused on developing and testing a Linux kernel module to monitor localhost traffic. It tracks packet flows via loopback interfaces (127.0.0.1 for IPv4 and ::1 for IPv6), classifies packets by IP protocol version, and builds a histogram of packet sizes.

---

## Project structure

m5_localhost_monitor/
├── localhost_monitor.c          # Kernel module source code
├── print_localhost_stats.sh     # Script to read /proc file
├── Makefile                     # Build instructions
├── README.md                    # Project description
└── podman/                      # Optional VM/container setup (Podman)


---

### 2. `podman` Folder
Includes scripts and configurations to set up a containerized environment for building and running the kernel and modules.

**Note:** For setup instructions, see the `README.md` inside the `podman` folder.

---

### 3. `tests` Folder
Contains:
- `vm`: The root filesystem used by the virtual machine to run the guest OS with the custom kernel. It also includes scripts to build, run, and connect to the VM.
- `scripts`: Collection of scripts for testing and various use cases within the VM environment.

---

## How to Get Started

1. Navigate to the `podman` folder:

   ```bash
   cd podman
   ```

2. Follow the instructions provided in the `README.md` within the `podman` folder to set up the containerized environment.

---

## Additional Notes
- After setting up the environment, you can build and install the kernel and modules using the provided make targets.
- Use the scripts inside the `tests/scripts` folder for testing specific functionalities within the VM.

---

## Summary
This repository is designed to facilitate customizing the Linux kernel, building kernel modules, and testing them within virtual machines, all orchestrated through containerized environments. Follow the provided instructions in each subfolder's `README.md` files to properly set up and operate the environment.

---

*For further assistance or questions, refer to the individual README files within each folder.*
