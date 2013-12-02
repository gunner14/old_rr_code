#!/bin/sh

# Start a service. This script is called remotely by maintenance tools automatically.

# Arguments: <service>

# Check arguments
if [ $# -ne 1 ]; then
	#echo "Usage: $(basename $0) <service>" > /dev/stderr;
	exit 1
fi

# Check service. Modify args on a per-service basis. E.g. modify
# service_vm_args for certain services that need specific Java VM parameters.

service_name=$1;
service_bean_xml="";
service_vm_args="-Xmn2g -Xms2g -Xmx2g"
#service_vm_args="-Xmn500m -Xms512m -Xmx512m";
service_start_class="com.renren.ad.sns.util.thrift.ThriftServerImplementor"
# Set service_listen_port to 0 if your service does not listen to any port.
# Assign it a non-zero value of a tcp port your service will listen. Leave it
# empty if you want the script to parse the port from the service_bean_xml
# file.
service_listen_port=;
case $service_name in
	"impr-service")
	service_bean_xml="etc/beans/impr-test.xml";
	service_listen_port="9098";
	memory_options="-Xms1g -Xmx1g";
    #memory_options="-Xms512m -Xmx512m";
	gc_options=" -XX:+PrintGCApplicationStoppedTime -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintGCApplicationConcurrentTime -XX:+UseParNewGC -Xloggc:logs/gc.log";
	service_vm_args="$memory_options $gc_options";
	;;
	
	"ranking-service")
	service_bean_xml="src/main/resources/beans/ranking-service.xml";
	service_listen_port="9630";
	memory_options="-Xms2g -Xmx2g";
    gc_options=" -XX:+PrintGCApplicationStoppedTime -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintGCApplicationConcurrentTime -XX:+UseParNewGC -Xloggc:logs/gc.log";
	service_vm_args="$memory_options $gc_options";
	;;

	"log-service")
	service_bean_xml="etc/beans/log.xml";
	service_listen_port="9099";
	memory_options="-Xms1g -Xmx1g";
	gc_options=" -XX:+PrintGCApplicationStoppedTime -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintGCApplicationConcurrentTime -XX:+UseParNewGC -Xloggc:logs/gc.log";
	service_vm_args="$memory_options $gc_options";
	;;
    "antiFraud-service")
    service_bean_xml="etc/beans/antifrauder.xml";
    service_listen_port="9096";
    memory_options="-Xms512m -Xmx512m";
    gc_options=" -XX:+PrintGCApplicationStoppedTime -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintGCApplicationConcurrentTime -XX:+UseParNewGC -Xloggc:logs/gc.log";
    service_vm_args="$memory_options $gc_options";
    ;;
	*)
	echo "Service not regconized: $service_name." > /dev/stderr;
	exit 1;
esac

# Set appropriate paths and args
proj_path=$(dirname $0)/..
pid_file="$proj_path/$service_name.pid";
port_file="$proj_path/$service_name.port";
stdout_file="$proj_path/logs/stdout";
stderr_file="$proj_path/logs/stderr";
vm_args="-server -verbose:gc -XX:PermSize=64M -XX:MaxPermSize=64M -XX:+UseConcMarkSweepGC -XX:+UseParNewGC -XX:-CMSParallelRemarkEnabled -XX:+UseConcMarkSweepGC -XX:+UseCMSCompactAtFullCollection -XX:SurvivorRatio=8 -XX:MaxTenuringThreshold=7 -XX:GCTimeRatio=19 -XX:CMSInitiatingOccupancyFraction=70 -XX:CMSFullGCsBeforeCompaction=0 -XX:+CMSClassUnloadingEnabled $service_vm_args";

# Check pid file
if [ -f $pid_file ]; then
	echo "Error: Service $service_name may be already started. Check the status first." > /dev/stderr;
	exit 1;
fi

# Write the port number into a file
if [ -z "$service_listen_port" ]; then
	# try to parse the port if not specified
	service_listen_port=$(
		cat $proj_path/$service_bean_xml | \
		  sed -n '/<bean id="server".*>/,/<\/bean>.*/p' | \
		  sed -n 's/.*<property name="port">.*<value>\(.*\)<\/value>.*/\1/p;
			  /.*<property name="port">/,/<\/property>/s/.*<value>\([0-9]*\)<\/value>.*/\1/p' );
fi

if [ -z "$service_listen_port" ]; then
	echo "Error: cannot read port number from $service_bean_xml. Syntax changed? " > /dev/stderr;
	exit 1;
fi

[ $service_listen_port -eq 0 ] && service_listen_port="N/A";
echo $service_listen_port > $port_file;

# backup stdout_file and stderr_file if they are not empty
[ -s $stdout_file ] && mv $stdout_file $stdout_file.$(date +%Y-%m-%d.%H%M)
[ -s $stderr_file ] && mv $stderr_file $stderr_file.$(date +%Y-%m-%d.%H%M)

# invoke the server.
#nohup java $vm_args -classpath 'classes:lib/*' $service_start_class $service_bean_xml server >$stdout_file 2>$stderr_file &
export MAVEN_OPTS="$vm_args"
mvn exec:java -e -Dexec.mainClass="$service_start_class" -Dexec.args="$service_bean_xml server" >$stdout_file 2>$stderr_file &

# write down the pid
pid=$!;
if [ -z $pid ]; then
	echo "Error: cannot start the service." > /dev/stderr;
	exit 1;
fi
echo $pid > $pid_file;
test -f $proj_path/$service_name.gcwarning && rm $proj_path/$service_name.gcwarning

# refresh heart-beat file if it is not empty
FILE_HEART_BEAT=logs/stat.log
[ -s $FILE_HEART_BEAT ] && touch $FILE_HEART_BEAT;

