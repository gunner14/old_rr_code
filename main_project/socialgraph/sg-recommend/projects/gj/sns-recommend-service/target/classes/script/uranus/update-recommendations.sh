#!/bin/bash

export PATH="/usr/kerberos/bin:$PATH"
export KRB5CCNAME=/tmp/krb5cc_pub_$$ 
trap kdestroy 0 1 2 3 5 15
kinit -k -t /data/uranus/krb5.keytab

m4_host="xiang.zhang@ZWDX256-30.opi.com"
m4_job_file="/home/xiang.zhang/renrengj-cf/start-cf-job.sh"

# run recommend hadoop job at M4
echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] updating starting..."
echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [begin] run M4 script."
ssh $m4_host "source ~/.bash_profile && $m4_job_file"
echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [end] run M4 script."


# pull recommend result from M4
echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [begin] pull result from M4."
rec_result_file="/home/xiang.zhang/renrengj-cf/data/cf-recommendation.dat"
dest_dir="/data/uranus/offline/cf"
dest_file="$dest_dir/cf-recommendation.dat"
dest_file_tmp="$dest_dir/cf-recommendation.dat.tmp"
scp $m4_host:$rec_result_file $dest_file_tmp
mv $dest_file_tmp $dest_file
echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [end] pull result from M4."
echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] updating successfully.\n\n"
