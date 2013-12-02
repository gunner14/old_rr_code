#! /bin/bash
if [ -e debug -a -d debug ]; then
  cd debug
  rm -rf *
else
  mkdir debug
  cd debug
fi

## 单元测试
echo "########"
echo "单元测试"
echo "########"

cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
ctest -D Experimental
ctest -D ExperimentalMemCheck
ctest -D ExperimentalSubmit

## 代码发布
echo "########"
echo "代码发布"
echo "########"

cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
make install
