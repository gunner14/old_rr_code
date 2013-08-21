#!/bin/bash
source /data/script/key/init.sh

sh /data/yinxin/scripts/monitor_guide_access/scp_remote_log.sh
sh /data/yinxin/scripts/monitor_guide_access/proc_log.sh
python /data/yinxin/scripts/monitor_guide_access/send.py
