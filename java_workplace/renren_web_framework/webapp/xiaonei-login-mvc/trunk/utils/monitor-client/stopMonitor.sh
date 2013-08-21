#!/bin/sh

ps -ef | grep "SCREEN -S monitor" |awk '{print $2}' | xargs kill -9
screen -wipe
