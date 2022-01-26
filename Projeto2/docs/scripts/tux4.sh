#!/bin/sh

ifconfig eth0 down
ifconfig eth1 down

# Configuring tux4
ifconfig eth0 up
ifconfig eth0 172.16.60.254/24

ifconfig eth1 up
ifconfig eth1 172.16.61.253/24

# Enabling forwarding in tuxes
echo 1 > /proc/sys/net/ipv4/ip_forward

# Enabling echo reply to broadcast request
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

# Set in tuxy4 the default gateway to the Cisco Router 
route add default gw 172.16.61.254

# To access the lab network through router, maybe not needed because of the default gw
route add -net 172.16.2.0/24 gw 172.16.61.254

# To verify if routes are okay
route -n




