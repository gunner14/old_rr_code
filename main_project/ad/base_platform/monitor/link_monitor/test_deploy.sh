#!/bin/bash

#for node in  ad58 ad59 ad60 ad61 ad62 ad64 ad65 ad66 ad67 ad68 ad69 ad70 ad71 ad72 #ad0 
#for node in ad72
#for node in ad1 ad2 ad4 ad5 ad6 ad7 ad8 ad9 ad10 ad11 ad12 ad13 ad14 ad15 ad16 ad20 ad22 ad25 ad26 ad27 ad28 ad29 ad30 ad31 ad34 ad35 ad36 ad37 ad38 ad39 ad40 ad41 ad42 ad47 ad48 ad49 ad53 ad54  ad58 ad59 ad60 ad61 ad62 ad64 ad65 ad66 ad67 ad68 ad69 ad70 ad71 ad72 #ad0 
#for node in adexpr3
for node in adexpr1 adexpr2 adexpr3
do
	echo "installing on ${node}"
	monitor_dir="/data/xce/monitor/link_monitor"
	ssh -o "StrictHostKeyChecking no" xce@${node}.xce "mkdir -p ${monitor_dir}"
	scp -o "StrictHostKeyChecking no" link_monitor_node.sh xce@${node}.xce:${monitor_dir}
	ssh -o "StrictHostKeyChecking no" xce@${node}.xce "crontab -l > ${monitor_dir}/cron; cp ${monitor_dir}/cron ${monitor_dir}/cron.bak; sed -i '/sh link_monitor_node.sh/d' ${monitor_dir}/cron; echo '*/10 * * * * cd ${monitor_dir} && sh link_monitor_node.sh >> link_monitor_node.log 2>&1' >> ${monitor_dir}/cron; crontab ${monitor_dir}/cron;"
done
