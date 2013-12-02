#!/bin/bash

MonitorInterval=1000
WarningInterval=300000
UpdateInterval=600000
SendEmail=1
SendSms=1

sh start_tripod_monitor.sh $MonitorInterval $WarningInterval $UpdateInterval $SendEmail $SendSms 
