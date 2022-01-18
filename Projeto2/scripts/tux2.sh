#!/bin/sh

ifconfig eth0 down

# Configuring tux2
ifconfig eth0 up
ifconfig eth0 172.16.61.1/24

# To access the lab network through router
route add -net 172.16.60.0/24 gw 172.16.61.253


route add default gw 172.16.61.254

# To verify if routes are okay
route -n




