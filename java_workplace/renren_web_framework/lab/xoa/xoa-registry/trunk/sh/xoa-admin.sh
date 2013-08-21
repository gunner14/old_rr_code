#!/bin/sh

#This script is used for lauch XoaAdmin Console.
#@author weibo.li@renren-inc.com
#@version 1.004

#check JAVA_HOME environment variable
if [ -z "$JAVA_HOME" ]; then
  echo "JAVA_HOME environment variable is not defined"
  echo "JAVA_HOME environment variable is needed to run this program"
  exit 1
fi

# resolve links - $0 may be a softlink
PRG="$0"

while [ -h "$PRG" ]; do
  ls=`ls -ld "$PRG"`
  link=`expr "$ls" : '.*-> \(.*\)$'`
  if expr "$link" : '/.*' > /dev/null; then
    PRG="$link"
  else
    PRG=`dirname "$PRG"`/"$link"
  fi
done

# Get standard environment variables
PRGDIR=`dirname "$PRG"`

# Only set CATALINA_HOME if not already set
[ -z "$CATALINA_HOME" ] && CATALINA_HOME=`cd "$PRGDIR/.." ; pwd`


# Config classpath
_CLASSPATH=.:$CLASSPATH:$CATALINA_HOME/ext-lib
for i in $CATALINA_HOME/lib/*.jar
do
    _CLASSPATH=$_CLASSPATH:"$i"
done

for i in $CATALINA_HOME/ext-lib/*.jar
do
    _CLASSPATH=$_CLASSPATH:"$i"
done

$JAVA_HOME/bin/java -DCATALINA_HOME=$CATALINA_HOME -classpath $_CLASSPATH com.renren.xoa.registry.tomcat.XoaAdminMain $@



