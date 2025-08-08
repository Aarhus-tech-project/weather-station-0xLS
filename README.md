# Weather Station

## Installing required libraries
```bash
apt install libpaho-mqttpp-dev libpaho-mqtt-dev libmysqlcppconn-dev libssl-dev build-essential cmake g++ libboost-system-dev mosquitto mosquitto-clients iw net-tools neofetch nginx docker.io
```

## Spinning up mysql8.0 in a docker
```bash
sh ./boot-mysql.sh
```
mysql should be accessible at `0.0.0.0:3306`with the following credentials
- username: `root`
- password: `password`
