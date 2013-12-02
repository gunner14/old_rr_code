sh get_nodelist.sh>nodelist
cat nodelist |tr "[:upper:]" "[:lower:]"|awk '{print host/$0@XIAONEI.OPI.COM}'> nodelist_k5
