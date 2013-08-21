#!/bin/sh

cd /data/httperror
cat ./logs/404.log|awk '{if($2=="500") print $6}'|awk -F ? '{print $1}'|sort|uniq -c|sort -nr|head -n 20|awk '{if($2!="") print $0}' > 500.txt

python 500mail.py
