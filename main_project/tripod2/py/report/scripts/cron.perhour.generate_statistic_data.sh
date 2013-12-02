#!/bin/bash

Time=`date -d "1 hour ago" +%Y-%m-%d-%H`
sh get_one_hour_statistic.sh $Time
