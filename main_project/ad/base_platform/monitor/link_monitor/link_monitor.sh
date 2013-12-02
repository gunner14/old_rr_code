#!/bin/bash
pwd=$(dirname $0) && cd ${pwd}

echo ================ [ $(date "+%F %T") BEGIN ] =======================

awk '
BEGIN {
	OFMT="%.2g";
	CONVFMT="%.2g";
}

ARGIND == 1 {
	if ($0 ~ /^#.*/)
		; # comment
	else if (NF == 0)
		; # blank line
	else
	{
		threshold[$1] = $2;
	}
}

ARGIND > 1 {
	if (FNR == 1)
	{
		file = ARGV[ARGIND];
		host = gensub("_record", "", "g", file);
		host = gensub("data/", "", "g", host);
	}

	link_num = $1;
	split($2, arr, "/");
	service = arr[2];
	if ((service in threshold) && (link_num > threshold[service]))
		print host, service, link_num >> "warnmsg/" service;
}' conf/threshold.conf data/*_record

cd warnmsg
thead="节点 服务 连接数"
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
	for i in memcached
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

			echo -e "${thead}\n""${content}" | awk -f ../print_table.awk >> email.html

			email_flag=1
			sms_flag=0
		fi
	done

	if ((email_flag == 1))
	then
		content=$(cat email.html)
		/data/home/yuxiang.huang/local/bin/python ../sendmail.py "${email_list}" "[Link监控]服务异常" "${content}"
	fi

	if ((sms_flag == 1))
	then
		for phone in ${sms_list//;/ }
		do
			echo "send sms ${phone}"
			wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=${phone}&message=[Link监控]服务异常.详情见邮件"
		done
	fi

done

cd ..

# rm data/*_record
mkdir data/$(date "+%d%H%M")_data
mv data/*_record data/$(date "+%d%H%M")_data
rm data/$(date -d "7 days ago" "+%d%H%M")_data

rm warnmsg/*

echo ================ [ $(date "+%F %T") END ] =======================
