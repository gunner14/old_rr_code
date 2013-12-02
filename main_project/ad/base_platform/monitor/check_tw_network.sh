#!/bin/bash

echo `date +%Y/%m/%d\ %H:%M`
for n in $(seq 10)
do
	curl --max-time 8 -o /dev/null -s -w"$n LVS BW VIP(60.29.242.224):\t%{size_download}\t%{http_code}\t%{time_connect}\t%{time_starttransfer}\t%{time_total}\n" 'http://60.29.242.224/bwad/ADIDViewer.aspx?adzoneid=100000000001&count=3&url=http://apps.renren.com/hongsejingjie?origin=50115&ubc=1000000_307620034|0|1976-01-01|36|4|0086130600000000|0_0086000000000000|0|0|0|0086130600000000'

	curl --max-time 8 -o /dev/null -s -w"$n BW Server(60.29.241.116):\t%{size_download}\t%{http_code}\t%{time_connect}\t%{time_starttransfer}\t%{time_total}\n" 'http://60.29.241.116/bwad/ADIDViewer.aspx?adzoneid=100000000001&count=3&url=http://apps.renren.com/hongsejingjie?origin=50115&ubc=1000000_307620034|0|1976-01-01|36|4|0086130600000000|0_0086000000000000|0|0|0|0086130600000000'

	curl --max-time 8 -o /dev/null -s -w"$n LVS FMN VIP(60.29.242.220):\t%{size_download}\t%{http_code}\t%{time_connect}\t%{time_starttransfer}\t%{time_total}\n" -H "Host: fmn.xnimg.cn" 'http://60.29.242.220/200.html'

done
