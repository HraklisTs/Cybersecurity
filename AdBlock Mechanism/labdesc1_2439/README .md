# Introduction
Tsaousidis Iraklis 
2015030150

In this assignment we developed a bash script, that uses Linux command iptables in order to create a simple adblocking mechanism that rejects packets coming from specific network advertising domains. Through the bash script we generate a set of firewall rules that block/reject access for specific network domain names.

After configuring the adblock rules, I visited some websites to test my tool and I noticed that although some ads were blocked, there were still some visible frames.
This was due to the fact that ads may be pop-ups or redirections, and our adblock mechanism has no effect on them. Also another reason could be that our list of domain names was short, so it may be more efficient if we blocked using the hosts files.

Resolve time for 500 domain names tested, approximately 1:45 minutes.

## How to build

``` c
sudo ./adblock.sh -domains : Configure adblock rules based on domain names of 'domainNames.txt' file.
sudo ./adblock.sh -ips : Configure adblock rules based on IP addresses of 'IPAddresses.txt' file.
sudo ./adblock.sh -save : Save rules to 'adblockRules' file.
sudo ./adblock.sh -load : Load rules from 'adblockRules' file.
sudo ./adblock.sh -list : List current rules.
sudo ./adblock.sh -reset : Reset rules to default (ACCEPT all).
sudo ./adblock.sh -help : Display help and exit.
```

## Implementation
``` c
function adBlock()
```
Through this function we resolve the domain names provided in a text file to the appropriate IP addresses, using Google DNS server,  with the network administration command-line tool "dig" for DNS lookup operations. After that, we configure adblock rules based on the domain names or based on the IP addresses from the appropriate text file, according to the arguments given to our tool ("-domains" or "-ips"). Furthermore, our tool has some extra options: 
-  "-save", which saves rules into 'adblockRules' file we create,
-  "-load", which loads rules from 'adblockRules' file,
-  "-list", which lists current rules,
-  "-reset", which resets rules to default settings (ACCEPT all),
-  "-help", which displays help and exits.


###  Helper functions or macros
---
``` c
function resolve() : Resolve the domain names provided to the appropriate IP addresses, using Google DNS server and command-line tool "dig".
```
