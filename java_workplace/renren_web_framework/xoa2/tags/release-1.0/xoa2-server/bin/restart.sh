test1=`echo $0 | awk -F/ 'BEGIN { ORS="/"; }  \
       { i = 1; while ( i < NF ) { print $i; i++;}}'`
#echo $test1
if [ -z "$test1" ]; then
  currentPath=`pwd`
else
  currentPath=`cd $test1; pwd`
fi
sh $currentPath/stop.sh
echo "restart xoa again"
sh $currentPath/start.sh
