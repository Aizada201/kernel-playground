# Localhost Traffic Monitor

This project focuses on developing and testing a Linux kernel module to monitor localhost traffic. It tracks packet flows via loopback interfaces (127.0.0.1 for IPv4 and ::1 for IPv6), classifies packets by IP protocol version and transport protocol, and builds a histogram of packet sizes. All statistics are made available via procfs and printed to the kernel log on module removal.

---

## How to Set Up

### Prerequisites

- Linux system with kernel headers installed  
- Git  
- Make  
- Root privileges  

### Setup Instructions

```bash
git clone https://github.com/YOUR_USERNAME/kernel-playground.git
cd kernel-playground/m5_localhost_monitor

make

sudo insmod localhost_monitor.ko

ping -c 2 127.0.0.1
ping -c 2 ::1

./print_localhost_stats.sh

sudo rmmod localhost_monitor
dmesg | tail -30
```

---

## Basic Level

### Goal

- Detect traffic to/from 127.0.0.1 (IPv4) and ::1 (IPv6)

### Implementation

- Uses Netfilter hooks to intercept packets on loopback interfaces  
- Logs each matching packet to the kernel log with `printk()`  

---

## Intermediate Level

### Goal

- Classify packets by IP version and protocol  
- Track packet size distribution  
- Export live stats to procfs  

### Implementation

- Counts packets by:  
  - IP version: IPv4, IPv6  
  - Protocol: TCP, UDP, ICMP, ICMPv6  
- Tracks packet sizes using histogram buckets:  
  - 0–127, 128–255, 256–511, 512–1023, 1024+  
- Exports statistics to `/proc/localhost_stats`  
- Prints summary to `dmesg` when unloaded  

---

## Example Output

### `/proc/localhost_stats`

```
==== Localhost Monitor Stats ====
IPv4 packets: 10
IPv6 packets: 4
TCP packets: 2
UDP packets: 3
ICMP packets: 5
ICMPv6 packets: 1
Packet size histogram (bytes):
0-127: 4
128-255: 3
256-511: 2
512-1023: 3
1024-65535: 2
```

### Kernel log summary

```
[localhost_monitor] Final stats: IPv4=10, IPv6=4, TCP=2, UDP=3, ICMP=5, ICMPv6=1
```

---

## Screenshots

![Module loaded and stats](path/to/your/screenshot1.png)  
![Procfs output](path/to/your/screenshot2.png)  
![Kernel log summary](path/to/your/screenshot3.png)

---

## Development Notes

### Design Choices

- Used Netfilter hooks for efficient packet inspection  
- Chose histogram buckets to cover typical packet sizes  
- Used procfs for easy user-space access to stats  

### Implementation

- IPv4/IPv6 and protocol classification in Netfilter hooks  
- Packet size histogram updated per packet  
- Procfs entry `/proc/localhost_stats` for live stats  
- Summary printed to kernel log on module unload  

### Challenges

- Ensuring compatibility with modern kernel APIs (`struct proc_ops`)  
- Testing both IPv4 and IPv6 traffic in a VM environment  

---

## Documentation

- [Google Doc with full report and screenshots](https://docs.google.com/document/d/1KW6rukKouOoXsEkoVlZb6jkgFjA_FYvWIC7n12Ah6Eg)

---

## License

This project inherits the license of the original [kernel-playground](https://github.com/netgroup/kernel-playground) repository.
