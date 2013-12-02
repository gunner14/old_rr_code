#!/bin/sh

# This script returns the status of the specified service. It is called
# remotely by maintenance tool automatically.

# Arguments: <service>

# The status of the specified service is returned as the return status of this
# script. Meaning of status:

# 80: "+Running"
# 81: "-Stopped"
# 82: "*Crashed"
# 83: "!Error"
# 84: "%ARunning"
# 85: '!Starting'
# 86: '$Attention' #take a look

S_RUNNING=80;
S_STOPPED=81;
S_CRASHED=82;
S_ERROR=83;
S_ARUNNING=84;
S_STARTING=85;
S_ATTENTION=86;

# Check arguments
if [ $# != 1 ]; then
	echo "Return the status of the specified service." > /dev/stderr;
	echo "Usage: $(basename $0) <service>" > /dev/stderr;
	echo "The result is returned as return status of the script." > /dev/stderr;
	exit 1;
fi

service_name="$1";
message_file="/dev/stdout";

# Check pid file

proj_path=$(dirname $0)/..
pid_file="$proj_path/$service_name.pid";

if [ ! -f $pid_file ]; then
	echo "STOPPED" >> $message_file;
	echo "The pid file $pid_file is not found." >> $message_file;
	exit $S_STOPPED;
fi

# Check the process id
pid=$(cat $pid_file);
if [ -z "$pid" ]; then
	echo "CRASHED" >> $message_file;
	echo "There is no pid in the file $pid_file." >> $message_file;
	exit $S_CRASHED;
fi

# Check the process
if [ ! -d /proc/$pid ]; then
	echo "CRASHED" >> $message_file;
	echo "The process with id $pid does not exist." >> $message_file;
	exit $S_CRASHED;
fi

# Check if the service error while starting or running
pattern_oom="java.lang.OutOfMemoryError: Java heap space";
if grep "$pattern_oom" "$proj_path/logs/stderr"; then
	echo "ERROR" >> $message_file;
	echo "The process is running but there is an OOM (out-of-memory) exception detected." >> $message_file;
	exit $S_ERROR;
fi

# Check if the service error while starting or running
if test -f "$proj_path/$service_name.gcwarning"; then
	echo "ERROR" >> $message_file;
    echo "there is gcwarning tag file" >> $message_file;
	exit $S_ERROR;
fi

# Check heart-beats, using logs/stat.log if it exists and is not empty. If the
# file was last modified 15 minitues ago, the service is considered dead.
FILE_HEART_BEAT=logs/stat.log
if [ -s $FILE_HEART_BEAT && -n "$(find $FILE_HEART_BEAT -mmin +15)" ]; then
	echo "ERROR" >> $message_file;
	echo "The file $FILE_HEART_BEAT was last modified 15 minitues ago." >> $message_file;
	exit $S_ERROR;
fi

# The service is actually running
echo "RUNNING" >> $message_file;
echo "PID=$pid" >> $message_file;
#echo "PORT=$port" >> $message_file;
exit $S_RUNNING;
