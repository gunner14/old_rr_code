#!/bin/bash
#由于无法上传静态文件，故把静态文件获取方法写成脚本

MYROOT=`pwd`
TARGET=$MYROOT/../../../../release-newarch/
CMAKEROOT=$MYROOT/../../../../cmake/
CMAKEFILE=bootstrap.cmake
PACKAGE=UserClient3G
HEADFILE=src/*.h
LOCAL_LIB=$MYROOT/$PACKAGE/lib
LOCAL_INCLUDE=$MYROOT/$PACKAGE/include
LIB=$(echo UtilCxx UtilSlice IceExt OceCxxAdapterUserNameAdapter OceCxxAdapterUserPassportAdapter OceCxxAdapterBuddyRelationReplicaAdapter;
  echo OceSliceReplicaCluster OceSliceUserBase OceSliceBuddyRelationCache OceSliceBuddyCommon ReplicaCluster $PACKAGE);

#进度打印函数
echox() {
  echo -e "\033[33m[UserClient3G]"$@"...\n---------------------------------------\e[0m"
}

#清理环境，保证发布最新的接口
rm -rf $LOCAL_LIB $LOCAL_INCLUDE
mkdir -p $LOCAL_LIB $LOCAL_INCLUDE && cp $MYROOT/$HEADFILE $LOCAL_INCLUDE 

#修改编译规则，优先依赖于log4cplus-1.0.4
#修改的准则是把1.0.4的头文件和库文件放到1.0.3的前面
cp $CMAKEROOT/$CMAKEFILE $CMAKEROOT$CMAKEFILE.bak
sed -i s/"INCLUDE_DIRECTORIES("/"INCLUDE_DIRECTORIES( \${XNS_ROOT_DIR}\/third-party\/log4cplus\/include"/g $CMAKEROOT/$CMAKEFILE
sed -i s/"\/usr\/local\/xce\/lib64"/"\${XNS_ROOT_DIR}\/third-party\/log4cplus\/lib \/usr\/local\/xce\/lib64"/g $CMAKEROOT/$CMAKEFILE

#编译依赖库
echox "Handling dependecy..."
cd $TARGET && cmake .
for i in $LIB; do
  lib="lib$i.a"
  rm -f $lib
done

for i in $LIB; do
  lib="lib$i.a"
  echox "Hadling $lib..."
  c=0
  while [ ! -f $lib ]; do
    c=`expr $c + 1`
    if [ $c -gt 30 ]; then
      echox "Complile falied"
      exit
    fi
    echox "Compliling $i round $c...";
    make $i -j10;
  done;
  mv $lib $LOCAL_LIB
done;

#把所有.a打成一个.a包
cd $LOCAL_LIB
ar x lib$PACKAGE.a && rm lib$PACKAGE.a
for i in $(echo `ls *.a|xargs`);do
  echo $i;
  ar x $i;
done
rm *.a && ar r lib$PACKAGE.a *.o && rm *.o

#测试
cd $MYROOT/$PACKAGE/sample
cmake . && make
if [ `./UserClient3GTest | wc -l` -ge "5" ];then
  echox "Test ok!";
  #清理编译文件
  ls | grep -vE "src|CMakeList" | xargs rm -rf
  #打包发布
  cd $MYROOT && tar jcf $PACKAGE.tar.bz2 $PACKAGE
  #清理发布文件
  cd $MYROOT/$PACKAGE && rm -rf lib include
else
  echox "Test failed!"
fi

#还原cmake规则文件
echox "Restore environment..."
mv $CMAKEROOT/$CMAKEFILE.bak $CMAKEROOT$CMAKEFILE
cd $TARGET && cmake .
echox "Done! See $PACKAGE.tar.bz2"
