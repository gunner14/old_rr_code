#!/bin/sh
echo ""
echo "----stage1: svn checkout"
echo ""
echo ">> rm -rf $1/trunk"
rm -rf $1/trunk
echo ">> rm -f $1/etc/service.list"
rm -f $1/etc/service.list
echo ">> svn co http://svn.xiaonei.com/oce/main_project/trunk"
svn co http://svn.xiaonei.com/oce/main_project/trunk
if [ $? -ne 0 ];then
	return 1;
else
	cat $1/trunk/cmake/start.cmake | grep "^ADD_OCESERVICE" | awk -F "\"" '{if($4==""){output=$2}else{output=$4};print $2" "output" lib"output".so" }' >> $1/etc/service.list
fi
