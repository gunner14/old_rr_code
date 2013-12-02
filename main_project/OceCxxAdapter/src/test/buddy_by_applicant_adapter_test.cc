#include "gtest/gtest.h"
#include <iostream>
#include "../BuddyByApplicantAdapter.h"
#include <time.h>


#define BASE_YEAR     1900  //localtime转换后的年份是相对与1900年的年份，需要加上1900修正为当前年份
#define BASE_MONTH    1     //localtime转换后的月份从0开始，需要加1


namespace xce {
namespace buddy {
namespace adapter {

//获取指定ID申请的用户列表和发出申请的时间并打印输出
TEST(BuddyByApplicantCacheAdapter, PrintAccepterListAndTime) {
  int test_id = 349440824;
  try {
    tm* pTm;
    int userId;
    long time;
    BuddyByApplicantDataPtr resultPtr = BuddyByApplicantCacheAdapter::instance().getAccepters(test_id, 0, -1);
    AccepterInfoSeq result = resultPtr->accepters;
    ::std::vector<AccepterInfo>::iterator iterator = result.begin();
    while(iterator != result.end()){
      AccepterInfo node = *iterator;
      userId = node.accepter;
      time = node.time;
      pTm = localtime(&time); //使用localtime转换时间格式
      printf("userId = %d, \ttime = %4d-%02d-%02d %02d:%02d:%02d \n", userId, \
          pTm->tm_year + BASE_YEAR, pTm->tm_mon + BASE_MONTH, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
      ++iterator;
    } //End of while
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::buddyapplycache



