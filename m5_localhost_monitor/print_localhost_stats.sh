if [ ! -f /proc/localhost_stats ]; then
	echo "/proc/localhost_stats not found. Is the kernel module loaded?"
	exit 1

fi

echo "==== Localhost Monitor Stats ===="
date
cat /proc/localhost_stats
