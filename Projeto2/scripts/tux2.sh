#!/bin/sh

ifconfig eth0 down

# Configuring tux2
ifconfig eth0 up
ifconfig eth0 172.16.61.1/24

# Configure the routes in tuxy2 so that they can reach tuxy3
route add -net 172.16.60.0/24 gw 172.16.61.253

# Set in tuxy2 the default gateway to the Cisco Router 
route add default gw 172.16.61.254

# To access the lab network through router, maybe not needed because of the default gw
route add -net 172.16.2.0/24 gw 172.16.61.254

# To verify if routes are okay
route -n




