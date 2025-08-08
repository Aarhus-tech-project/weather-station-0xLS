# Weather Station

### WARNING: Program made using debian12

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
chmod +x ./mqtt_mysql_logger
./mqtt_mysql_logger
```
