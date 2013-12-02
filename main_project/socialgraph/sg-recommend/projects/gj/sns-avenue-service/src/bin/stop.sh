#!/bin/bash

# Stop the specified service. This script is called remotely by the
# maintenance tools.

# Arguments: [-f] <service>

# If '-f' is given, the service is forced to stop by a `kill -9' command.


# Check arguments

if (($# < 1 || $# > 2)); then
    echo "Stop the specified service. Use '-f' to force." > /dev/stderr;
    echo "Usage: $(basename $0) [-f] <service>" > /dev/stderr;
    exit 1;
fi

service_name="";
force_option=""; # passed to kill
if (($# == 1)); then
    service_name=$1;
else
    service_name=$2;
    if [[ $1 == "-f" ]]; then
        force_option="-9";
    else
        echo "Expected '-f' but found '$1'." > /dev/stderr;
        exit 1;
    fi
fi

# Read pid file

proj_path=$(dirname $0)/..
pid_file="$proj_path/$service_name.pid";
port_file="$proj_path/$service_name.port";
pid=
if [[ -f $pid_file ]]; then
    pid=$(cat $pid_file);
else
    echo "STOPPED";
    echo "The service '$service_name' is already stopped. Cannot find the file $pid_file.";
    exit 0;
fi

if [[ -z "$pid" || ! -d /proc/$pid ]]; then
    echo "STOPPED";
    echo "The service '$service_name' is already stopped. Cannot find its process.";
    rm -rf $port_file $pid_file;
    exit 0;
fi

if [[ ! -O /proc/$pid ]]; then
    echo "Error: the process with id $pid of the service $service_name is not owned by the user $USER." > /dev/stderr;
    exit 1;
fi

# Dump threads and stacks for the convenience of debugging. Sending a sigquit
# signal will cause the java process to dump out a bunch of threads/stacks
# information to the stdout.

kill -SIGQUIT $pid;

# Kill the process of the specified service

if kill $force_option $pid; then
    rm -rf $pid_file $port_file;
    echo "STOPPED";
else
    echo "Error when stopping the service. Try '-f' option." > /dev/stderr;
    exit 1;
fi

test -f $proj_path/$service_name.gcwarning && rm $proj_path/$service_name.gcwarning
