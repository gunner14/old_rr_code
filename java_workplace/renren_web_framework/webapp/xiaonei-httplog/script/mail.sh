#!/bin/sh

cd /data/httperror
cat ./logs/404.log|awk '{if($2=="404") print $6}'|awk -F ? '{print $1}'|sort|uniq -c|sort -nr|head -n 20|awk '{if($2!="") print $0}' > list.txt

python mail.py
