## 1. Project Overview

This project implements a Linux kernel module to monitor and classify localhost (loopback) traffic (IPv4: 127.0.0.1/8, IPv6: ::1). The module detects such traffic, classifies packets by protocol (TCP, UDP, ICMP, ICMPv6), tracks packet size distribution, and exports statistics via procfs for user-space access. The project is designed for the kernel-playground environment.

---

## 2. How to Set Up

**Step 1: Create the Project Directory and Files**
- Inside your cloned `kernel-playground` repository, create a folder named `m5_localhost_monitor`.
- Inside this folder, create:
  - `loopback_monitor.c` — the kernel module source code.
  - `Makefile` — for building the kernel module.


**Step 2: Build the Module**
```sh
cd m5_localhost_monitor
make
```
This will produce `loopback_monitor.ko`.

---

## 3. Basic Level

**Goal:**  
Detect and log traffic to/from 127.0.0.1 (IPv4) and ::1 (IPv6).

**Implementation:**  
- The kernel module uses Netfilter hooks to inspect packets.
- It logs a message to the kernel log whenever a loopback packet is detected.

**After `make`:**
- Load the module:
  ```sh
  sudo insmod loopback_monitor.ko
  ```
- Generate traffic:
  ```sh
  ping -c 2 127.0.0.1
  ping -c 2 ::1
  ```
- Check the kernel log for detection messages:
  ```sh
  sudo dmesg | tail -30
  ```

**Example Results:**
<img width="1210" height="271" alt="image" src="https://github.com/user-attachments/assets/af65e5cf-74b9-4e84-8fea-3c888d6ee20f" />
<img width="1213" height="208" alt="image" src="https://github.com/user-attachments/assets/b5d5cdd1-86a6-46fc-858e-be0e79668921" />

And in the kernel log:
```
[loopback_monitor] IPv4 loopback packet detected
[loopback_monitor] IPv6 loopback packet detected
```


---

## 4. Intermediate Level

**Goal:**  
Classify packets by IP version and protocol, track packet size distribution, and export statistics via procfs.

**Changes in `loopback_monitor.c`:**
- Added counters for IPv4, IPv6, TCP, UDP, ICMP, and ICMPv6.
- Implemented a histogram for packet size distribution.
- Exported statistics to `/proc/localhost_stats`.
- Printed a summary to the kernel log on module unload.
- A helper script (print_localhost_stats.sh) is provided to easily read and display the statistics from /proc/localhost_stats.

**Further Commands:**
- Generate more traffic (ICMP, TCP, UDP, etc.).
- View live stats:
  ```sh
  ./print_localhost_stats.sh
  ```
  or
  ```sh
  cat /proc/localhost_stats
  ```


**Example Results:**
<img width="1209" height="292" alt="image" src="https://github.com/user-attachments/assets/82d93165-4684-4c55-bbba-4e48179d9a0b" />

And in the kernel log:
```
[loopback_monitor] Final stats: IPv4=10, IPv6=4, TCP=2, UDP=3, ICMP=5, ICMPv6=1
```
**Unload the module and view the summary:**
```
  sudo rmmod loopback_monitor
  sudo dmesg | tail -30
```
---
