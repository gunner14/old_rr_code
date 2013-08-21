#!/bin/bash
## 1. 将项目重新编译打包
mvn clean package

## 2. 将 jar 包和对应的 jobconf.xml 拷贝到对应环境
# cp target/xoa2-scheduler-demo-1.0-SNAPSHOT-job.jar ~/server/liuyuan
# cp conf/* ~/server/liuyuan

## 3. 运行测试程序
# ssh hadoop@10.11.18.13 "hadoop-1.0.1/bin/hadoop fs -rmr liuyuan/output"
# ssh hadoop@10.11.18.13 "hadoop-1.0.1/bin/hadoop jar ~/liuyuan/xoa2-scheduler-demo-1.0-SNAPSHOT-job.jar com.renren.xoa2.scheduler.XoaScheduler ~/liuyuan/jobconf_test.xml liuyuan/input liuyuan/output"
