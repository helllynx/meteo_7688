# Simple Air quality station with linkit 7688 

For ssh access connect to WIFI of linkit first. Then use this command:

```bash
ssh  -oKexAlgorithms=+diffie-hellman-group1-sha1 -oHostKeyAlgorithms=+ssh-rsa root@192.168.100.1
```

where 192.168.100.1 is address of linkit device, please check it first.

Put script write_data_to_memory.py to /root then add next line to `/etc/rc.local`

```bash
((python script.py)&)&
((python -m SimpleHTTPServer 9000)&)&
```