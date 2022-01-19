#!/bin/sh

ifconfig eth0 down

# Configuring tux3
ifconfig eth0 up
ifconfig eth0 172.16.60.1/24

# Configure the routes in tuxy3 so that they can reach tuxy2
route add -net 172.16.61.0/24 gw 172.16.60.254

# Set tux3 a default gateway to tux4 
route add default gw 172.16.60.254

# To verify if routes are okay
route -n



