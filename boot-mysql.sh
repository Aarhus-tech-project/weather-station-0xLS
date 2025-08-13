docker run -d \
  --name mysql \
  -e MYSQL_ROOT_PASSWORD=password \
  -e TZ=Europe/Copenhagen \
  -p 0.0.0.0:3306:3306 \
  --restart unless-stopped \
  mysql:8.0 \
  --default-time-zone='Europe/Copenhagen'