#!/bin/bash

#for node in  ad58 ad59 ad60 ad61 ad62 ad64 ad65 ad66 ad67 ad68 ad69 ad70 ad71 ad72 #ad0 
#for node in ad72
for node in ad1 ad2 ad4 ad5 ad6 ad7 ad8 ad9 ad10 ad11 ad12 ad13 ad14 ad15 ad16 ad20 ad22 ad25 ad26 ad27 ad28 ad29 ad30 ad31 ad34 ad35 ad36 ad37 ad38 ad39 ad40 ad41 ad42 ad47 ad48 ad49 ad53 ad54  ad58 ad59 ad60 ad61 ad62 ad64 ad65 ad66 ad67 ad68 ad69 ad70 ad71 ad72 #ad0 
do
	echo "installing on ${node}"
	scp -o "StrictHostKeyChecking no" node_data.sh xce@${node}.xce:/data/xce/Ad/bin
	ssh -o "StrictHostKeyChecking no" xce@${node}.xce "cd /data/xce/Ad/bin; sh node_data.sh"
#	ssh -o "StrictHostKeyChecking no" xce@${node}.xce "crontab -l > /data/xce/Ad/bin/cron; sed -i '/monitor_node.sh/d' /data/xce/Ad/bin/cron; echo '*/5 sleep 60;* * * * cd /data/xce/Ad/bin && sh monitor_node.sh >> monitor_node.log 2>&1' >> /data/xce/Ad/bin/cron; crontab /data/xce/Ad/bin/cron;"
done
