#!/bin/bash

#for node in  ad58 ad59 ad60 ad61 ad62 ad64 ad65 ad66 ad67 ad68 ad69 ad70 ad71 ad72 #ad0 
#for node in ad74
#for node in ad1 ad2 ad4 ad5 ad6 ad7 ad8 ad9 ad10 ad11 ad12 ad13 ad14 ad15 ad16 ad20 ad22 ad25 ad26 ad27 ad28 ad29 ad30 ad31 ad34 ad35 ad36 ad37 ad38 ad39 ad40 ad41 ad42 ad47 ad48 ad49 ad53 ad54  ad58 ad59 ad60 ad61 ad62 ad64 ad65 ad66 ad67 ad68 ad69 ad70 ad71 ad72 ad74 #ad0 
#广告机器列表
#for node in ad10 ad11 ad12 ad13 ad14 ad15 ad16 ad24 ad25 ad26 ad27 ad29 ad30 ad31 ad34 ad35 ad36 ad37 ad38 ad39 ad40 ad41 ad42 ad46 ad47 ad48 ad49 ad53 ad54 ad58 ad59 ad60 ad62 ad63 ad66 ad7 ad72 ad75 ad8 ad9



#for node in ad10 ad11 ad12 ad13 ad14 ad15 ad16 ad22 ad24 ad25 ad27 ad29 ad30 ad31 ad34 ad35 ad36 ad37 ad38 ad39 ad40 ad41 ad42 ad46 ad47 ad48 ad49 ad5 ad53 ad54 ad58 ad59 ad60 ad63 ad66 ad7 ad72 ad75 ad8 ad9

#edm和trigger的机器列表
for node in edm0 edm1 edm2 edm3 trigger1 trigger10 trigger11 trigger12 trigger13 trigger14 trigger2 trigger3 trigger4 trigger6 trigger7 trigger8 trigger9
#for node in ad76

	#for node in adexpr3
do
	echo "installing on ${node}"
#	monitor_dir="/data/xce/monitor/warn_monitor"
#	ssh -o "StrictHostKeyChecking no" xce@${node}.xce "mkdir -p ${monitor_dir}"
	scp -o "StrictHostKeyChecking no" warn_monitor_node.sh xce@${node}.xce:${monitor_dir}
#	ssh -o "StrictHostKeyChecking no" xce@${node}.xce "crontab -l > ${monitor_dir}/cron; cp ${monitor_dir}/cron ${monitor_dir}/cron.bak; sed -i '/sh warn_monitor_node.sh/d' ${monitor_dir}/cron; echo '*/3 * * * * sleep 60; cd ${monitor_dir} && sh warn_monitor_node.sh >> warn_monitor_node.log 2>&1' >> ${monitor_dir}/cron; crontab ${monitor_dir}/cron;"
done
