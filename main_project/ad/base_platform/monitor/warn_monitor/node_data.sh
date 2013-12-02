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

data_dir=$$.data
mkdir -p ${data_dir}
find /data/xce/Ad/log/* -maxdepth 0 -type d \( -name "Ad*" -o -name "Edm*" \) |
while read service_dir
do
	prefix="${service_dir}/oce_log."
	
	service=$(basename ${service_dir})
	service_var=${service//-/_}

	for day in 0 1 2
	do
		date=$(date -d "${day} days ago" "+%Y-%m-%d")
		echo ${service_var}:${date}
		grep "WARN" ${prefix}${date}-* > ${data_dir}/${service_var}.${date}.warn
		grep "Ice::Timeout" ${prefix}${date}-* > ${data_dir}/${service_var}.${date}.ice_timeout
		grep "ConnectTimeout" ${prefix}${date}-* > ${data_dir}/${service_var}.${date}.connect_timeout
		grep "FATAL" ${prefix}${date}-* > ${data_dir}/${service_var}.${date}.fatal
	done
done

data_tgz=$(hostname).tgz
tar czvf ${data_tgz} ${data_dir}

scp -o "StrictHostKeyChecking no" ${data_tgz} yuxiang.huang@SJSWT45-75.opi.com:/data/home/yuxiang.huang/warn_monitor/analysis/

rm -rf ${data_dir}
rm -rf ${data_tgz}

echo ================ [ $(date "+%F %T") END ] =======================
