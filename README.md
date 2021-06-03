This repo contains the solution for the LongGé project for the course of [Internet Inside (EITN30)](https://www.eit.lth.se/index.php?ciuid=1392&coursepage=10001&L=1) at LTH. As a glimpse, the basic requirement is achieved by using the [forked driver of nRF24L01+ by TMRh20](https://nrf24.github.io/RF24/). The IP layer is implemented with the library of [RF24Network](https://nrf24.github.io/RF24Network/) from the same author. Using this, we are able to achieve end-to-end bandwidth up to 1.05 Mbps, measured with iperf using udp.

![Setup:](https://github.com/syafiq/inuti/blob/main/images/setup.jpg)

## Installation

Follow the instructions [here](https://nrf24.github.io/RF24/md_docs_linux_install.html) to get the base library on your computer. 
Once RF24, RF24Gateway, RF24Network, and RF24Mesh are installed, each of the BaseStation and MobileUnit can be compiled by calling `make` from each directory under `ip`.

To run the BaseStation, execute `sudo ./BaseStation`. To run the MobileUnit, execute `sudo ./MobileUnit`. 

## Performance Measurement
### Delay
```pi@pi1:~ $ ping 10.1.3.33
PING 10.1.3.33 (10.1.3.33) 56(84) bytes of data.
64 bytes from 10.1.3.33: icmp_seq=1 ttl=64 time=16.1 ms
64 bytes from 10.1.3.33: icmp_seq=2 ttl=64 time=13.2 ms
64 bytes from 10.1.3.33: icmp_seq=3 ttl=64 time=17.6 ms
64 bytes from 10.1.3.33: icmp_seq=4 ttl=64 time=15.7 ms
64 bytes from 10.1.3.33: icmp_seq=5 ttl=64 time=16.8 ms
64 bytes from 10.1.3.33: icmp_seq=6 ttl=64 time=21.6 ms
64 bytes from 10.1.3.33: icmp_seq=7 ttl=64 time=21.9 ms
64 bytes from 10.1.3.33: icmp_seq=8 ttl=64 time=13.4 ms
^C
--- 10.1.3.33 ping statistics ---
8 packets transmitted, 8 received, 0% packet loss, time 18ms
rtt min/avg/max/mdev = 13.183/17.051/21.946/3.071 ms
```

### iPerf
#### TCP
```
pi@pi1:~ $ iperf -c 10.1.3.33
------------------------------------------------------------
Client connecting to 10.1.3.33, TCP port 5001
TCP window size: 23.8 KByte (default)
------------------------------------------------------------
[  3] local 10.1.3.1 port 54978 connected with 10.1.3.33 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-10.1 sec  33.2 KBytes  27.1 Kbits/sec
```
#### UDP
```
pi@pi1:~ $ iperf -c 10.1.3.33 -u
------------------------------------------------------------
Client connecting to 10.1.3.33, UDP port 5001
Sending 1470 byte datagrams, IPG target: 11215.21 us (kalman adjust)
UDP buffer size:  176 KByte (default)
------------------------------------------------------------
[  3] local 10.1.3.1 port 41058 connected with 10.1.3.33 port 5001
[  3] WARNING: did not receive ack of last datagram after 10 tries.
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-10.0 sec  1.25 MBytes  1.05 Mbits/sec
[  3] Sent 2 datagrams
```

### For Fun
With the current set up, it is possible to stream 144p video over HTTP, but I had to be extremely patient. Setting up a streaming server over UDP **might** be feasible though. 
