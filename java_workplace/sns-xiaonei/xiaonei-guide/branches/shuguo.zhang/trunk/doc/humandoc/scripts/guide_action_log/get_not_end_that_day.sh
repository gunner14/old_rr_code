#!/bin/bash
D=$1
echo "select user_id,last_active_time from guide_log where user_type='AAF' and fillinfo_arrive_isornot = 1 and insert_time >= '$D 00:00:00' and insert_time < '$D 23:59:59' and leadend_arrive_isornot = 1 and last_active_time > '$D 23:59:59'order by last_active_time;" | /usr/local/webserver/mysql/bin/mysql -umyuser -p12345  inshion_db >> not_end_that_day.$D.txt
