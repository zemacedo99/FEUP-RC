#!/bin/sh

ifconfig eth0 down
ifconfig eth1 down

# Configuring tux4
ifconfig eth0 up
ifconfig eth0 172.16.60.254/24

ifconfig eth1 up
ifconfig eth1 172.16.61.253/24


# To access the lab network through router
route add -net 172.16.2.0/24 gw 172.16.61.254

route add default gw 172.16.61.254

# To verify if routes are okay
route -n




