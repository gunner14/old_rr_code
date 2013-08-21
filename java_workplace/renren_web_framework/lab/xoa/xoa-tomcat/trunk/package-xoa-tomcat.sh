#!/bin/bash

#This script is using to package xoa-tomcat.
#@author weibo.li@renren-inc.com
#@version 1.0


#function to delete all .svn folders
function dfs(){
for file in ` ls -A $1 `
do
  if [ -d $1"/"$file ]
  then
    if [ $file = ".svn" ]
    then
      echo "Deleting "$1"/"$file
      rm -rf $1"/"$file
    else
      dfs $1"/"$file
    fi
  fi
done
}

if [ -z $1 ]
then
  echo "Usage: sh package-xoa-tomcat.sh [xoa-tomcat-VERSION.tar.gz]"
  exit 1
fi

INIT_PATH="."

#delete .project file
echo "Deleting .project"
rm -f ".project"

#delete all .svn folders
dfs $INIT_PATH  

#chmod +x to all .sh files
for file in ` ls  $INIT_PATH/bin/*.sh `
do
  echo "chmod +x "$file
  chmod +x $file
done

#package xoa-tomat
cd ..
echo "Packaging "$1
tar -czvf $1 xoa-tomcat
