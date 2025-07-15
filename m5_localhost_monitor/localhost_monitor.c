#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/string.h>
#include <linux/in6.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aizada");
MODULE_DESCRIPTION("M5 - Localhost Traffic Monitor (127.0.0.1, ::1)");

static struct nf_hook_ops nfho_v4;
static struct nf_hook_ops nfho_v6;

//taks2 counters
static unsigned long ipv4_count = 0;
static unsigned long ipv6_count = 0;
static unsigned long tcp_count = 0;
static unsigned long udp_count = 0;
static unsigned long icmp_count = 0;
static unsigned long icmpv6_count = 0;


#define HIST_BUCKETS 5
static unsigned long pkt_size_hist[HIST_BUCKETS] = {0};
static const int hist_ranges[HIST_BUCKETS+1] = {0, 128, 256, 512, 1024, 65536};

static struct proc_dir_entry *proc_entry;

static void update_histogram(unsigned int len) {
	int i;
	for (i=0; i < HIST_BUCKETS; ++i) {
	if (len >= hist_ranges[i] && len < hist_ranges[i+1]) {
		pkt_size_hist[i]++;
		break;
		}
	}
}

static unsigned int loopback_monitor_v4(void *priv,
                              		struct sk_buff *skb,
                           	   	const struct nf_hook_state *state) 
{
	struct iphdr *iph;
	if (!skb)
		return NF_ACCEPT;
	iph = ip_hdr(skb);
	if(!iph)
		return NF_ACCEPT;
	if (((ntohl(iph->saddr) & 0xFF000000) == 0x7F000000) || 
		((ntohl(iph->daddr) & 0xFF000000) == 0X7F000000)) {

		ipv4_count++;
		update_histogram(skb->len);
		switch (iph->protocol) {
			case IPPROTO_TCP:
				tcp_count++;
				break;
			case IPPROTO_UDP:
				udp_count++;
				break;
			case IPPROTO_ICMP:
				icmp_count++;
				break;
		}

            	printk(KERN_INFO "[loopback_monitor] IPv4 loopback packet detected\n");
        }
	return NF_ACCEPT;
}

static unsigned int loopback_monitor_v6(void *priv,
                                        struct sk_buff *skb,
                                        const struct nf_hook_state *state) 
{
        struct ipv6hdr *ip6h;
	struct in6_addr loopback = IN6ADDR_LOOPBACK_INIT;
        if (!skb)
                return NF_ACCEPT;
        ip6h = ipv6_hdr(skb);
        if(!ip6h)
                return NF_ACCEPT;
        if (!memcmp(&ip6h->saddr, &loopback, sizeof(struct in6_addr)) || 
           	!memcmp(&ip6h->daddr, &loopback, sizeof(struct in6_addr))) {

                ipv6_count++;
                update_histogram(skb->len);
                if (ip6h->nexthdr ==IPPROTO_TCP) {
                        tcp_count++;
		} else if (ip6h->nexthdr == IPPROTO_UDP) {
			udp_count++;
		} else if (ip6h->nexthdr == IPPROTO_ICMPV6) {
			icmpv6_count++;
		}
		printk(KERN_INFO "[loopback_monitor] IPv6 loopback packet detected\n");
        }
        return NF_ACCEPT;
}



static int localhost_stats_show(struct seq_file *m, void *v) {
	int i;
	seq_printf(m, "IPv4 packets: %lu\n", ipv4_count);
        seq_printf(m, "IPv6 packets: %lu\n", ipv6_count);
        seq_printf(m, "TCP packets: %lu\n", tcp_count);
        seq_printf(m, "UDP packets: %lu\n", udp_count);
        seq_printf(m, "ICMP packets: %lu\n", icmp_count);
        seq_printf(m, "ICMPv6 packets: %lu\n", icmpv6_count);
	seq_puts (m, "Packet size histogram (bytes):\n");
	for (i=0; i < HIST_BUCKETS; ++i) {
		seq_printf(m, "%d-%d: %lu\n", hist_ranges[i], hist_ranges[i+1]-1, pkt_size_hist[i]);
	}
	return 0;
}


static int localhost_stats_open(struct inode *inode, struct file *file) {
	return single_open(file, localhost_stats_show, NULL);
}

static const struct proc_ops localhost_stats_fops = {
	.proc_open    = localhost_stats_open,
	.proc_read    = seq_read,
	.proc_lseek   = seq_lseek,
	.proc_release = single_release,
};


//static const struct file_operations localhost_stats_fops = {
//	.owner = THIS_MODULE,
//	.open = localhost_stats_open,
//	.read = seq_read,
//	.llseek = seq_lseek,
//	.release = single_release,
//};


static int __init localhost_monitor_init(void) 
{
    	nfho_v4.hook = loopback_monitor_v4;
    	nfho_v4.hooknum = NF_INET_PRE_ROUTING;
    	nfho_v4.pf = PF_INET;
    	nfho_v4.priority = NF_IP_PRI_FIRST;
    	nf_register_net_hook(&init_net, &nfho_v4);

        nfho_v6.hook = loopback_monitor_v6;
        nfho_v6.hooknum = NF_INET_PRE_ROUTING;
        nfho_v6.pf = PF_INET6;
        nfho_v6.priority = NF_IP_PRI_FIRST;
        nf_register_net_hook(&init_net, &nfho_v6);

	proc_entry = proc_create("localhost_stats", 0, NULL, &localhost_stats_fops);
	printk(KERN_INFO "[loopback_monitor] Module loaded\n");
    	return 0;
}


static void __exit localhost_monitor_exit(void) 
{
      	nf_unregister_net_hook(&init_net, &nfho_v4);
    	nf_unregister_net_hook(&init_net, &nfho_v6);
	if (proc_entry)
		remove_proc_entry("localhost_stats", NULL);

        printk(KERN_INFO "[loopback_monitor] Module unloaded\n");
	printk(KERN_INFO "[loopback_monitor] Final stats: IPv4=%lu, IPv6=%lu, TCP=%lu, UDP=%lu, ICMP=%lu,ICMPv6=%lu\n",
	 ipv4_count, ipv6_count, tcp_count, udp_count, icmp_count, icmpv6_count);
}

module_init(localhost_monitor_init);
module_exit(localhost_monitor_exit);


