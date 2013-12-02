#!/bin/bash
pwd=$(dirname $0) && cd ${pwd}

echo ================ [ $(date "+%F %T") BEGIN ] =======================

# prepare for scp
if [[ -f /etc/krb5.keytab ]]
then
	export PATH="/usr/kerberos/bin:$PATH"
	export KRB5CCNAME=/tmp/krb5cc_pub_$$
	trap kdestroy 0 1 2 3 5 15
	kinit -k -t /etc/krb5.keytab
fi

[[ -e lastread ]] && source lastread
this_time=$(date "+%Y-%m-%d-%H")
echo "last_time=${this_time}" > lastread
if [[ -z "${last_time}" ]]
then
	same_hour=1
elif [[ ${last_time} == ${this_time} ]]
then
	same_hour=1
else
	same_hour=0
fi

node=$(hostname)
#find /data/xce/[Ad|Edm|Trigger]/log/* -maxdepth 0 -type d \( -name "Ad*" -o -name "Edm*" \) |
find /data/xce/Ad/log/* /data/xce/Edm/log/* /data/xce/Trigger/log/* -maxdepth 0 -type d \( -name "Ad*" -o -name "Edm*" -o -name "Index*" \) |
while read service_dir
do
	log=${service_dir}/oce_log
	if [[ ! -e ${log} ]]
	then
		continue
	fi

	mtime=$(ls -l --time-style="+%Y-%m-%d-%H" ${log} | awk '{print $6}')
	if [[ "${mtime}" != "${this_time}" ]]
	then
		continue
	fi
	
	tmpfile=$$.tmp
	service=$(basename ${service_dir})
	service_var=${service//-/_}
	last_read=${!service_var:-0}
	if ((same_hour == 0))  # 时间改变，原来的文件已经被重命名
	then
		last_hour=$(date -d "1 hour ago" "+%Y-%m-%d-%H")
		last_hour_log="${log}.${last_hour}"

		echo "read ${last_hour_log}: from ${last_read} to end"
		sed "1, ${last_read}d" ${last_hour_log} > ${tmpfile}
		last_read="-"$(wc -l < ${tmpfile})

		cat ${log} >> ${tmpfile}
	else
		sed "1, ${last_read}d" ${log} > ${tmpfile}
	fi

	warn=$(grep WARN ${tmpfile} | grep -v "bad lexical cast" | grep -v "give up putting" |grep -v "AdZonePool::CheckNeedSort" | wc -l)
	ice_timeout=$(grep "Ice::Timeout" ${tmpfile} | wc -l)
	connect_timeout=$(grep "ConnectTimeout" ${tmpfile} | wc -l)
	fatal=$(grep "FATAL" ${tmpfile} | wc -l)
	total=$(wc -l < ${tmpfile})

	rm ${tmpfile}

	echo "${log}: from ${last_read} to $((last_read + total))"

	echo ${node} ${service} ${warn} ${ice_timeout} ${connect_timeout} ${fatal} ${total} >> $$.record
	echo ${service_var}=$((last_read + total)) >> lastread
done

scp -o "StrictHostKeyChecking no" $$.record yuxiang.huang@SJSWT45-75.opi.com:/data/home/yuxiang.huang/warn_monitor/data/$(hostname)_record
rm $$.record

echo ================ [ $(date "+%F %T") END ] =======================
