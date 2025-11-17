#!/bin/bash

echo "Instalando dependências..."

mkdir temp

cd temp

sudo wget https://dev.mysql.com/get/mysql-apt-config_0.8.36-1_all.deb

sudo dpkg -i mysql-apt-config_0.8.36-1_all.deb

sudo apt-get update

sudo apt install libmysqlcppconn-dev

cd ..

rm -rf temp

echo "Fim"
