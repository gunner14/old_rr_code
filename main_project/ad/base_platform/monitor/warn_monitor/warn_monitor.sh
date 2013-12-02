#!/bin/bash
pwd=$(dirname $0) && cd ${pwd}

echo ================ [ $(date "+%F %T") BEGIN ] =======================

awk -f warn_monitor.awk conf/threshold.conf data/*_record

cd warnmsg
title_warn_email="WARN"
title_ice_timeout_email="Ice::Timeout"
title_connect_timeout_email="ConnectTimeout"
title_fatal_email="FATAL"
thead_warn_email="节点 服务 WARN行数 总行数 WARN占比 统计时间"
thead_ice_timeout_email="节点 服务 Ice::Timeout行数 总行数 Ice::Timeout占比 统计时间"
thead_connect_timeout_email="节点 服务 ConnectTimeout行数 总行数 ConnectTimeout占比 统计时间"
thead_fatal_email="节点 服务 FATAL行数 总行数 统计时间"
sed '/^#.*/d' ../conf/user.conf | while read email_list sms_list subscribe_list
do
	email_flag=0
	sms_flag=0

	cat << EOF > email.html
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type"	content="text/html;	charset=utf-8">
<style type="text/css">
table {
	border: 2px solid #888;
	border-collapse: collapse;
}
td {
	vertical-align:text-top;
	padding:8px;
	background-color:#efefef;
	border:2px solid #888;
}
</style>
</head>
<body>
EOF

	subscribe_list=${subscribe_list%;}
	for i in warn_email ice_timeout_email connect_timeout_email fatal_email
	do
		[[ ! -e ${i} ]] && continue

		if [[ ${subscribe_list} == "ALL" ]]
		then
			content=$(cat ${i})
		else
			content=$(grep -E ${subscribe_list//;/|} ${i})
		fi

		if [[ -n "${content}" ]]
		then
			title=title_${i}
			echo "<h2>${!title}</h2>" >> email.html

			thead=thead_${i}
			echo -e "${!thead}\n""${content}" | awk -f ../print_table.awk >> email.html

			email_flag=1
		fi
	done

	if ((email_flag == 1))
	then
		content=$(cat email.html)
		/data/home/yuxiang.huang/local/bin/python ../sendmail.py "${email_list}" "[Warn监控]服务异常" "${content}"
	fi

	for i in warn_sms ice_timeout_sms connect_timeout_sms fatal_sms
	do
		[[ ! -e ${i} ]] && continue

		if [[ ${subscribe_list} == "ALL" ]]
		then
			content=$(cat ${i})
		else
			content=$(grep -E ${subscribe_list//;/|} ${i})
		fi

		if [[ -n "${content}" ]]
		then
			sms_flag=1
		fi
	done

	if ((sms_flag == 1))
	then
		for phone in ${sms_list//;/ }
		do
			wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=${phone}&message=Warn Monitor:Service Abnormal,please check mail."
		done
	fi

done

cd ..

# rm data/*_record
mkdir data/$(date "+%d%H%M")_data
mv data/*_record data/$(date "+%d%H%M")_data
rm -rf data/$(date -d "7 days ago" "+%d%H%M")_data

rm warnmsg/*_email warnmsg/*_sms

echo ================ [ $(date "+%F %T") END ] =======================
