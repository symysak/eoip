Changed EoIPv6 header behavior

eoip
---

This is an implement of MikroTik's [EoIP](http://wiki.mikrotik.com/wiki/Manual:Interface/EoIP)/EoIPv6 tunnel using TAP. EoIP (Ethernet over IP) and EoIPv6 (Ethernet over IPv6) are MikroTik RouterOS's Layer 2 tunneling protocol.

The EoIP protocol encapsulates Ethernet frames in the GRE (IP protocol number 47) packets, and EoIPv6 protocol encapsulates Ethernet frames in the EtherIP (IP protocol number 97) packets.

### Supported Systems

- Linux
- OpenBSD*
- FreeBSD*
- Darwin

\* For some reason that I don't yet understand, specifying a local address of an interface on BSD-Based systems (except Darwin) will make `bind()` "Can't assign requested address." Please use `0.0.0.0` as the local address on these systems.

### Install

```
# git clone https://github.com/nat-lab/eoip
# cd eoip
# make
# make install
```

### Usage

```
Usage: eoip [ OPTIONS ] IFNAME { remote RADDR } { local LADDR } [ id TID ]
                               [ mtu MTU ] [ uid UID ] [ gid GID ] [ fork ]
where: OPTIONS := { -4 | -6 }
```

The parameters are pretty self-explanatory. `RADDR` = remote address, `LADDR` = local address, `TID` = tunnel ID, and `OPTIONS` can be either `-4` (EoIP) or `-6` (EoIPv6), EoIP will be used if unset. IFNAME, LADDR and RADDR are required fields, but there are not any parameter check whatsoever, so make sure you do it right.

Optionally you might choose to set MTU, set UID/GID of the process, or fork process to the background. If you set `fork`, `eoip` will fork to background and print PID to stdout.

Notice that `IFNAME` does not matter if you are using a non-Linux system, since `IFNAME` of TAP can't be set by us on the non-Linux system. Also, if you are using BSD, make sure to bring the TAP interface up as soon as you can. On BSD/Darwin, reading TAP file descriptor won't block if TAP is down, and this use up CPU cycles pretty quickly.

### Example

On MikroTik:

```
[user@mikrotik] > /interface eoip add local-address=172.17.0.1 name=eoip-1 remote-address=172.17.0.2 tunnel-id=100
```

On the other side:

```
# eoip -4 tap1 local 172.17.0.2 remote 172.17.0.1 id 100
```

Now `tap1` is connected to `eoip-1` on MikroTik, don't forget to bring `tap1` up.

### Protocol

EoIP uses IP Protocol 47, which is the same as GRE.

Here's the packet format of EoIP:

```
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       GRE FLAGS 0x20 0x01     |      Protocol Type  0x6400    | = "\x20\x01\x64\x00"
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Encapsulated frame length   |           Tunnel ID           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Ethernet frame...                                             |
```

(noticed that encapsulated frame length is in BE while Tunnel ID is in LE)

EoIPv6 is much simpler. It use IP Protocol 97, which is the same as EtherIP:

```
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  EoIP HEADER  | Ethernet frame...                             |
```

Header part of EoIPv6 are similar to [RFC3378](https://tools.ietf.org/html/rfc3378). The 12 reserved bits in the EtherIP header are now Tunnel ID. MikroTik also swaps the first four bits with second four bits in the EtherIP header, so the header looks like this:

```
0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|               |               |                               |
|  TID Part 1   |    VERSION    |          TID Part 2           |
|               |               |                               |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
```

where `VERSION` = ``"\x03"``.

### Contribute/Bugs

Feel free to send PR or report issue on the Github project page at:

https://github.com/nat-lab/eoip

### Acknowledgement

<del>My waifu</del> [@amphineko](https://github.com/amphineko) for making me de-punk this project :)

### Licenses

MIT
