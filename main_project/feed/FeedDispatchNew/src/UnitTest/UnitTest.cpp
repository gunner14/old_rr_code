#include"UnitTest.h"
#include"../FeedDispatchNewI.h"
#include"../DbHelper.h"
#include"../ExpressionParser.h"
#include "feed/FeedRedisClient/src/FeedIndex/FeedIndexRedisClient.h"
#include<time.h>
#include<iostream>
#include "LogWrapper.h"
#include "ServiceI.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>

using namespace xce::feed;
using namespace std;
void FeedDispatchNewTest::SetUp() {
  ConnectionPoolManager::instance().initialize();
}
void FeedDispatchNewTest::TearDown() {
}

TEST_F(FeedDispatchNewTest,GetFeedIndex) {
  FeedIndexPtr index = new FeedIndex();
  index->feed = 1;
  index->source = 1209101040256883736;
  index->newsMerge = 1209101040256883736;
  index->miniMerge = 1209101040256883736;
  index->psource = 1209101040256883736;
  index->actor = 260773925;
  index->time = time(NULL);
  index->stype = 9102;
  index->ptype = 9102;
  
  bool r = xce::feed::FeedIndexRedisClient::instance().AddIndex(index);
  EXPECT_TRUE(r);

  indexSeq = FeedDispatchNewI::instance().GetFeedIndex(1209101040256883736,9102,260773925); 
  EXPECT_TRUE(indexSeq.size());

  std::vector<FeedIndexRedisClient::IndexItem> value;
  r = xce::feed::FeedIndexRedisClient::instance().GetIndex(1209101040256883736,9102,value);
  EXPECT_TRUE(r);

  r = xce::feed::FeedIndexRedisClient::instance().RemoveIndex(1209101040256883736,9102);
  EXPECT_TRUE(r);

  xce::feed::FeedIndexRedisClient::instance().GetIndex(1209101040256883736,9102,value);
  EXPECT_FALSE(value.size());
}

TEST_F(FeedDispatchNewTest,ExpressionFriend) {
  set<int> resultP,resultM,resultA; 
  Parser parserP,parserM,parserA;

  string expr = "f(260773925)";
  parserP.Init(expr);
  parserP.Tokenize();
  resultP = parserP.CalculateIds();
  EXPECT_TRUE(resultP.size());
  cout<<expr<<" size:"<<resultP.size()<<endl;

  expr = "f(260773925)-f(260773925)";
  parserM.Init(expr);
  parserM.Tokenize();
  resultM = parserM.CalculateIds();
  EXPECT_EQ(0,resultM.size());
  cout<<expr<<" size:"<<resultM.size()<<endl;

  expr = "f(260773925)&f(260773925)";
  parserA.Init(expr);
  parserA.Tokenize();
  resultA = parserA.CalculateIds();
  EXPECT_EQ(resultP.size(),resultA.size());
  cout<<expr<<" size:"<<resultA.size()<<endl;
}

TEST_F(FeedDispatchNewTest,ExpressionGuangJie) {
  set<int> resultP,resultM,resultA; 
  Parser parserP,parserM,parserA;

  string expr = "j(260773925)";
  parserP.Init(expr);
  parserP.Tokenize();
  resultP = parserP.CalculateIds();
  EXPECT_TRUE(resultP.size());
  cout<<expr<<" size:"<<resultP.size()<<endl;

  expr = "j(260773925)-j(260773925)";
  parserM.Init(expr);
  parserM.Tokenize();
  resultM = parserM.CalculateIds();
  EXPECT_EQ(0,resultM.size());
  cout<<expr<<" size:"<<resultM.size()<<endl;

  expr = "j(260773925)&j(260773925)";
  parserA.Init(expr);
  parserA.Tokenize();
  resultA = parserA.CalculateIds();
  EXPECT_EQ(resultP.size(),resultA.size());
  cout<<expr<<" size:"<<resultA.size()<<endl;
}
TEST_F(FeedDispatchNewTest,ExpressionXiaoZu) {
  set<int> resultP,resultM,resultA; 
  Parser parserP,parserM,parserA;

  string expr = "z(700005005)";
  parserP.Init(expr);
  parserP.Tokenize();
  resultP = parserP.CalculateIds();
  EXPECT_TRUE(resultP.size());
  cout<<expr<<" size:"<<resultP.size()<<endl;

  expr = "z(700005005)-z(700005005)";
  parserM.Init(expr);
  parserM.Tokenize();
  resultM = parserM.CalculateIds();
  EXPECT_EQ(0,resultM.size());
  cout<<expr<<" size:"<<resultM.size()<<endl;

  expr = "z(700005005)&z(700005005)";
  parserA.Init(expr);
  parserA.Tokenize();
  resultA = parserA.CalculateIds();
  EXPECT_EQ(resultP.size(),resultA.size());
  cout<<expr<<" size:"<<resultA.size()<<endl;
}
TEST_F(FeedDispatchNewTest,ExpressionMiniSite) {
  set<int> resultP,resultM,resultA; 
  Parser parserP,parserM,parserA;

  string expr = "x(671210520)";
  parserP.Init(expr);
  parserP.Tokenize();
  resultP = parserP.CalculateIds();
  EXPECT_TRUE(resultP.size());
  cout<<expr<<" size:"<<resultP.size()<<endl;

  expr = "x(671210520)-x(671210520)";
  parserM.Init(expr);
  parserM.Tokenize();
  resultM = parserM.CalculateIds();
  EXPECT_EQ(0,resultM.size());
  cout<<expr<<" size:"<<resultM.size()<<endl;

  expr = "x(671210520)&x(671210520)";
  parserA.Init(expr);
  parserA.Tokenize();
  resultA = parserA.CalculateIds();
  EXPECT_EQ(resultP.size(),resultA.size());
  cout<<expr<<" size:"<<resultA.size()<<endl;
}
TEST_F(FeedDispatchNewTest,ExpressionCheWen) {
  set<int> resultP,resultM,resultA; 
  Parser parserP,parserM,parserA;
  string expr = "c(199000005)";
  parserP.Init(expr);
  parserP.Tokenize();
  resultP = parserP.CalculateIds();
  EXPECT_TRUE(resultP.size());
  cout<<expr<<" size:"<<resultP.size()<<endl;

  expr = "c(199000005)-c(199000005)";
  parserM.Init(expr);
  parserM.Tokenize();
  resultM = parserM.CalculateIds();
  EXPECT_EQ(0,resultM.size());
  cout<<expr<<" size:"<<resultM.size()<<endl;

  expr = "c(199000005)&c(199000005)";
  parserA.Init(expr);
  parserA.Tokenize();
  resultA = parserA.CalculateIds();
  EXPECT_EQ(resultP.size(),resultA.size());
  cout<<expr<<" size:"<<resultA.size()<<endl;
}
TEST_F(FeedDispatchNewTest,ExpressionTag) {
  set<int> resultP,resultM,resultA; 
  Parser parserP,parserM,parserA;
  string expr = "t(725851)";
  parserP.Init(expr);
  parserP.Tokenize();
  resultP = parserP.CalculateIds();
  EXPECT_TRUE(resultP.size());
  cout<<expr<<" size:"<<resultP.size()<<endl;

  expr = "t(725851)-t(725851)";
  parserM.Init(expr);
  parserM.Tokenize();
  resultM = parserM.CalculateIds();
  EXPECT_EQ(0,resultM.size());
  cout<<expr<<" size:"<<resultM.size()<<endl;

  expr = "t(725851)&t(725851)";
  parserA.Init(expr);
  parserA.Tokenize();
  resultA = parserA.CalculateIds();
  EXPECT_EQ(resultP.size(),resultA.size());
  cout<<expr<<" size:"<<resultA.size()<<endl;
}

