#!/bin/bash

domainNames="domainNames.txt"
IPAddresses="IPAddresses.txt"
adblockRules="adblockRules"

# Specify DNS server
dnsserver="8.8.8.8"

# Function to get IP address
function resolve() {

    if [[ ! -f "$domainNames" ]]; then
        echo "File not found..."
        exit -1
    fi

    while IFS= read -r domain; do
        dig @${dnsserver} ${domain} +short +tries=1 +time=6 | grep '[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*$' &>> IPAddresses.txt
    done < domainNames.txt

}


function adBlock() {
    if [ "$EUID" -ne 0 ];then
        printf "Please run as root.\n"
        exit 1
    fi

    if [ "$1" = "-domains"  ]; then
        # Configure adblock rules based on the domain names of $domainNames file.
       resolve &

       pid=$!
       wait $pid

       if [[ "$?" -eq "0" ]]; then
            echo "Done..."
       else
            echo "DNS resolution failed!"
       fi

        for ip in `cat IPAddresses.txt`; do
            iptables -A INPUT -s ${ip} -j REJECT 
            iptables -A FORWARD -s ${ip} -j REJECT
            iptables -A OUTPUT -s ${ip} -j REJECT
        done
                    
    elif [ "$1" = "-ips"  ]; then
        # Configure adblock rules based on the IP addresses of $IPAddresses file.
        for ip in `cat IPAddresses.txt`; do
            iptables -A INPUT -s ${ip} -j REJECT
            iptables -A FORWARD -s ${ip} -j REJECT
            iptables -A OUTPUT -s ${ip} -j REJECT 
        done
        
    elif [ "$1" = "-save"  ]; then
        # Save rules to $adblockRules file.
        iptables-save -f adblockRules
        
    elif [ "$1" = "-load"  ]; then
        # Load rules from $adblockRules file.
        iptables-restore < adblockRules
        
    elif [ "$1" = "-reset"  ]; then
        # Reset rules to default settings (i.e. accept all).
        iptables -P INPUT ACCEPT
        iptables -P FORWARD ACCEPT
        iptables -P OUTPUT ACCEPT 
        iptables -F

        
    elif [ "$1" = "-list"  ]; then
        # List current rules.
            iptables -L -nv
        
    elif [ "$1" = "-help"  ]; then
        printf "This script is responsible for creating a simple adblock mechanism. It rejects connections from specific domain names or IP addresses using iptables.\n\n"
        printf "Usage: $0  [OPTION]\n\n"
        printf "Options:\n\n"
        printf "  -domains\t  Configure adblock rules based on the domain names of '$domainNames' file.\n"
        printf "  -ips\t\t  Configure adblock rules based on the IP addresses of '$IPAddresses' file.\n"
        printf "  -save\t\t  Save rules to '$adblockRules' file.\n"
        printf "  -load\t\t  Load rules from '$adblockRules' file.\n"
        printf "  -list\t\t  List current rules.\n"
        printf "  -reset\t  Reset rules to default settings (i.e. accept all).\n"
        printf "  -help\t\t  Display this help and exit.\n"
        exit 0
    else
        printf "Wrong argument. Exiting...\n"
        exit 1
    fi
}

adBlock $1
exit 0