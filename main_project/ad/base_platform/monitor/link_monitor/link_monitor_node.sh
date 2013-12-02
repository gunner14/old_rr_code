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

node=$(hostname)
netstat -np | grep ESTABLISHED | awk '{print $7}' | sort | uniq -c > $$.record

scp -o "StrictHostKeyChecking no" $$.record yuxiang.huang@SJSWT45-75.opi.com:/data/home/yuxiang.huang/link_monitor/data/$(hostname)_record
rm $$.record

echo ================ [ $(date "+%F %T") END ] =======================
