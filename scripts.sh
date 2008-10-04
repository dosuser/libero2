modprobe iptable_filter
modprobe ip_queue
echo 1 >/proc/sys/net/ipv4/ip_forward

iptables -t nat -F
iptables -t mangle -F
iptables -t filter -F
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT
iptables -A FORWARD -j QUEUE
