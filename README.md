# Weather Station

### INFO: Program made & tested using Debian12 (bookworm)

## Installing required libraries
```bash
apt install libpaho-mqttpp-dev libpaho-mqtt-dev libmysqlcppconn-dev libssl-dev build-essential cmake g++ libboost-system-dev mosquitto mosquitto-clients iw net-tools neofetch nginx docker.io mysql-common sudo
```

## Spinning up mysql8.0 in a docker
```bash
sh ./boot-mysql.sh
```
mysql should be accessible at `0.0.0.0:3306`with the following credentials
- username: `root`
- password: `password`

## Spinning up the weather station subscriber
```bash
sh ./build.sh
./mqtt_mysql_logger
```

## Grafana

```bash
sudo apt install -y apt-transport-https software-properties-common wget curl gnupg2

sudo mkdir -p /etc/apt/keyrings
wget -q -O - https://apt.grafana.com/gpg.key | sudo gpg --dearmor -o /etc/apt/keyrings/grafana.gpg
echo "deb [signed-by=/etc/apt/keyrings/grafana.gpg] https://apt.grafana.com stable main" | sudo tee /etc/apt/sources.list.d/grafana.list

sudo apt update
sudo apt install -y grafana

sudo systemctl daemon-reexec
sudo systemctl enable grafana-server
sudo systemctl start grafana-server
```

### NGINX config for grafana

```nginx
location /grafana/ {
    proxy_pass http://localhost:3000/;
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;
    proxy_http_version 1.1;
    proxy_buffering off;
    proxy_request_buffering off;
    proxy_set_header Connection "";
    rewrite ^/grafana/(.*) /$1 break;
}
```

### Grafana settings for nginx

```ini
http_addr = 0.0.0.0
root_url = %(protocol)s://%(domain)s:%(http_port)s/grafana
serve_from_sub_path = true
```

## Mosquitto configuration
```conf
bind_address 0.0.0.0
allow_anonymous true
```

## General configuration

Machine the code was tested on has been setup with Debian12. <br />
Tailscale has been installed on the machine to allow for connecting outside the network. <br />
When not using tailscale to connect with, the machine is on h4prog network on the IP 192.168.102.254

MySQL has been setup through docker instead of just being installed on the machine due to mysql not having updated to support Bookwork (Debian12) yet.

Connecting to the machine has been done with SSH & vscode ssh remotes (using ssh keypairs).

### SSH Configuration

```conf
PermitRootLogin yes
PubkeyAuthentication yes
AuthorizedKeysFile      .ssh/authorized_keys .ssh/authorized_keys2
```

## Arduino image
![Test](image.png)