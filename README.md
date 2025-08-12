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
