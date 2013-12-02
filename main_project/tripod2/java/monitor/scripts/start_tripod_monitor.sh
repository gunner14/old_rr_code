#!/bin/bash

if [ $# -lt 3 ];then
  echo "Usage: $0 MonitorInterval WarningInterval UpdateInterval SendEmail SendSms"
  exit
fi

MonitorInterval=$1
WarningInterval=$2
UpdateInterval=$3
SendEmail=$4
SendSms=$5

Java=/data/xce/java
LogPath="../log"
LogFileName="tripod_monitor_log"
mkdir -p $LogPath
LogFile=$LogPath/$LogFileName

$Java -cp ../JavaMonitor/dependency/*:../JavaMonitor/tripod-monitor-1.0-SNAPSHOT.jar xce.tripod.monitor.TripodMonitor $MonitorInterval $WarningInterval $UpdateInterval $SendEmail $SendSms >> $LogFile  
