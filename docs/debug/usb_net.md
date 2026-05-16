---
orphan: true
---
# Configure USB Network Interface

We need USB NIC to communicate over the USB using network protocols like NFS.

Currently only networking interface we support is `ifup/ifdown`.

## ifup/ifdown

Set a static IP for the USB gadget interface by adding the interface to `/etc/network/interfaces`:
```text
allow-hotplug enxf8dc7a000001
iface enxf8dc7a000001 inet static
    address 192.168.7.1/24
    gateway 192.168.7.1
    hwaddress ether f8:dc:7a:00:00:01
```

The value `enxf8dc7a000001` is an interface name. On Debian it is derived from the NIC initial configuration:
- `en` -> Ethernet
- `x` -> external (non-PCI, usually USB)
- `f8dc7a000001` -> derived from the device MAC address

It can differ on your machine. Check `dmesg` to find the NIC name on your system.

Once the interface is configured restart networking:
```bash
sudo systemctl restart networking
```
