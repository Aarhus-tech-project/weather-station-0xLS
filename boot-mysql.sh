docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=password -p 0.0.0.0:3306:3306 --restart unless-stopped mysql:8.0
