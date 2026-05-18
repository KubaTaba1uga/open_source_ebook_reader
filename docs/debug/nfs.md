---
orphan: true
---
# Set up NFS server

Install the NFS server package:
```bash
sudo apt-get install nfs-kernel-server
```

Bind the NFS server to the USB network interface by editing `/etc/default/nfs-kernel-server`:
```text
RPCNFSDPRIORITY=0
NEED_SVCGSSD=""
RPCMOUNTDOPTS="--port 20048 --bind 192.168.7.1"
```

Export the NFS directory to the STM32MP135F board
(client IP `192.168.7.2`) by editing `/etc/exports`:
```text
/srv/nfs/ 192.168.7.2(rw,sync,no_root_squash,no_subtree_check)
```
