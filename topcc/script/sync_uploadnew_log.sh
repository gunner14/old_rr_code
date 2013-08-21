#!/bin/sh

# source environment
export PATH="/usr/local/bin:/usr/kerberos/bin:$PATH"
export KRB5CCNAME=/tmp/krb5cc_pub_$$
trap kdestroy 0 1 2 3 5 15
kinit -k -t /etc/krb5.keytab

# download
yesterday=`date -d last-day +%Y-%m-%d`
localtmp="/tmp/${yesterday}"
mkdir -p ${localtmp}
sh /data/release/dep.sh -f /data/release/newhost -n :/opt/upload_service/bin/log/new.log.${yesterday} ${localtmp}/upload_new

# upload
ssh -T -o "StrictHostKeyChecking no" qinghui.liu@10.5.18.185 "mkdir -p /home/qinghui.liu/hsync/${yesterday}"
scp ${localtmp}/* qinghui.liu@10.5.18.185:/home/qinghui.liu/hsync/${yesterday}
# cleanup
rm -rf ${localtmp}
# delete log 7 days ago
sevendaysago=`date --date="7 days ago" +%F`
sh /data/release/dep.sh -f /data/release/newhost -l "rm /opt/upload_service/bin/log/new.log.${sevendaysago}" 
