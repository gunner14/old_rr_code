#!bin/bash
test1=`echo $0 | awk -F/ 'BEGIN { ORS="/"; }  \
       { i = 1; while ( i < NF ) { print $i; i++;}}'`
#echo $test1

if [ -z "$test1" ]; then
  xoapidFile="./xoapid_file"
  if [ -z "$XOAServer_HOME" ]; then
    export XOAServer_HOME=`cd ..; pwd`;
  fi
else
  currentPath=`cd $test1; pwd`
# echo $currentPath

  xoapidFile="$currentPath/xoapid_file"
  
  if [ -z "$XOAServer_HOME" ]; then
    export XOAServer_HOME=`echo $currentPath/..`;
  fi
  #echo $XOAServer_HOME
fi

if [ -e "$xoapidFile" ]; then
  echo "不要重复启动同一个xoa服务"
  exit 0;
fi

if [ -z "$JAVA_HOME" ]; then
  export JAVA_HOME=`/opt/j2sdk/`;
fi
  #echo $JAVA_HOME

if [ -z "$XOAServer_MAINCLASS" ]; then
  XOAServer_MAINCLASS=com.renren.xoa2.server.Bootstrap
fi
#echo $XOAServer_MAINCLASS

XOAServer_LIB="$XOAServer_HOME/xoaapp/WEB-INF/lib $XOAServer_HOME/lib $XOAServer_HOME/conf"
#echo $XOAServer_LIB


for jarDir in $XOAServer_LIB; do
        for jarFile in `ls $jarDir`; do
                CLASS_PATH=$jarDir/$jarFile:$CLASS_PATH
        done
done
CLASS_PATH=$XOAServer_HOME/xoaapp/WEB-INF/classes:$CLASS_PATH

#JAVA_OPTS="-Dlog4j.configuration=file:$XOAServer_HOME/conf/log4j.properties";
JAVA_OPTS="-Dlog4j.configuration=file:$XOAServer_HOME/conf/log4j.xml";

DateTime=`date '+%Y-%m-%d'`
$JAVA_HOME/bin/java $JAVA_OPTS -cp $CLASS_PATH $XOAServer_MAINCLASS $XOAServer_HOME/xoaapp/WEB-INF/xoa.xml 1>> $XOAServer_HOME/logs/stdout.log.$DateTime 2>> $XOAServer_HOME/logs/stderr.log.$DateTime &  

xoapid=`ps -ef|awk '$3=='"$$ "'{print $2}'`
#xoapid=`ps aux | grep SCREEN | awk '{print $2}'`
#echo $xoapid

echo "服务启动成功"
echo $xoapid > $xoapidFile
