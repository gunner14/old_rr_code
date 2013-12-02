        if [ -f /etc/krb5.keytab ]; then
        export PATH="/usr/kerberos/bin:$PATH"
        export KRB5CCNAME=/tmp/krb5cc_pub_$$
        trap kdestroy 0 1 2 3 5 15
        kinit -k -t /etc/krb5.keytab
        fi
ping=$(ssh root@10.3.23.79 "ps -ef|grep "pingd\.xml"|grep -v grep|wc -l")
message="ping server down,please go to 10.3.23.79 and check"
if [[ ${ping} -eq 0 ]] ; then
	echo `date +%Y/%m/%d\ %H:%M` $message
        wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=18600883672&message=${message}";
        wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=18610489065&message=${message}";
        wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=18600574510&message=${message}";
        wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=18210295797&message=${message}";
        wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=18600276158&message=${message}";
        wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=13581697175&message=${message}";
fi;
echo "check done"
