#!bin/sh
if [ $# -lt 1 ];then
  echo "Usage: $0 ResultFile"
  exit
fi

ResultFile=$1
SmsSenderPythonClient="./send_sms.py"


Message=""
while read line 
do
  if [ "$Message" == "" ];then
    Message="$line"
  else 
    Message="$Message\n$line"
  fi
done < $ResultFile

#PhoneNumbers="13488666455"
PhoneNumbers="13488666455 18668158675 15001024430 13426085643"
#PhoneNumbers="13426085643"

for n in $PhoneNumbers
do
   echo "$SmsSenderPythonClient -c $n $Message"
   $SmsSenderPythonClient -c $n $Message

done

