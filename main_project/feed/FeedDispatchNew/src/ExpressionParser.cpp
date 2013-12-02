/*
 * test.cpp
 *
 *  Created on: Apr 8, 2010
 *      Author: yejingwei
 */
#include "ExpressionParser.h"
#include <iostream>
#include <sstream>
#include <boost/tuple/tuple.hpp>
#include <exception>
#include <set>
#include <stdexcept>
#include <ostream>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include <QueryRunner.h>
#include <DbDescriptor.h>
#include "ServiceI.h"
#include "FriendRankCacheAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
#include "ad/gj_cache/src/UserCacheClientPlus.h"
using namespace std;
using namespace xce::feed;
using namespace xce::socialgraph;
using namespace com::xiaonei::xce;
using namespace xce::buddy;
using namespace MyUtil;

set<int> Parser::CalculateIds() {
  MCE_DEBUG("Parser::CalculateIds()");
  size_t index = 0;
  //  TempResultPtr temp_result = new TempResult();//保存中间结果
  set<int> result;
  vector<FactorPtr>::iterator fit = factors_.begin();
  if(factors_.size()==0) {
    MCE_WARN("Parser::CalculateIds --> factors NULL")
    return result;
  }
  (*fit)->Plus(result);//对于第一个factor，加上一个空的ids，没有影响
  MCE_DEBUG("Parser::CalculateIds --> After calculating factor :" << index << ",result size = " << result.size());
  ++fit;
  ++index;
  vector<Op>::iterator oit = ops_.begin();
  while (fit != factors_.end()) {

    char op = (*oit).op_type();
    switch (op) {
    case '+':
      (*fit)->Plus(result);
      break;
    case '-':
      if(result.empty()){ //空的话，－结果还是空
        break;
      }
      (*fit)->Minus(result);
      break;
    case '&':
      if(result.empty()){//空的话，&结果还是空
        break;
      }
      (*fit)->And(result);
      break;
    }
    MCE_DEBUG("Parser::CalculateIds --> After calculating factor :" << index << ",result size = " << result.size());

    ++oit;
    ++fit;
    ++index;
  }
  return result;
}

bool Parser::Init(string & expr) {
  MCE_DEBUG("Parser::Init --> expression = " << expr);
  expr_str_ = expr;
  return true;
}

vector<FactorPtr> Parser::Tokenize() {
  MCE_INFO("Parser::Tokenize --> expr = " << expr_str_ );
  if(expr_str_.size() == 0) {
    MCE_WARN("Parser::Tokenize --> expr NULL");
    return factors_;
  }
  size_t pos = 0;
  while (pos < expr_str_.size()) {
    char tmpchar = expr_str_[pos];
    if (kOpTypes.find(tmpchar) != string::npos) {//字符是操作符
      Op tmpOp(tmpchar);
      ops_.push_back(tmpOp);
      ++pos;
    } else if (kFactorTypes.find(tmpchar) != string::npos) {//字符是因子类型符号
      factors_.push_back(ScanFactor(pos));
    } else {
      ostringstream oss;
      oss << "Parser::Tokenize() --> wrong charactor : '" << tmpchar
          << "',at position:" << pos << ",expression:" << expr_str_;
      string s = oss.str();
      throw logic_error(s);
    }
  }

  ostringstream foss;
  foss << "Parser::Tokenize --> factors are :";
  for (vector<FactorPtr>::iterator sit = factors_.begin(); sit
      != factors_.end(); ++sit) {
  //  if ((*sit)->type() != 'J')
      foss << (*sit)->type() << " " << (*sit)->param_id() << ",";
   // else
    //  foss << (*sit)->type() << " " << (*sit)->param_idL() << ",";

  }
  MCE_DEBUG(foss.str());
  ostringstream eoss;
  eoss << "Parser::Tokenize --> operators are :";
  if (ops_.size() == 0) {
    eoss << " empty ";
  } else {
    for (vector<Op>::iterator sit = ops_.begin(); sit != ops_.end(); ++sit) {
      eoss << sit->op_type() << ",";
    }
  }
  MCE_DEBUG(eoss.str());
  return factors_;
}

FactorPtr Parser::ScanFactor(size_t & pos) {
  char tmpchar = expr_str_[pos];
  /*  MCE_DEBUG("Parser::ScanFactor() --> tmpchar = "  <<  tmpchar);*/
  switch (tmpchar) {
  case 'f': {
    return new FriendFactor(tmpchar, ScanInteger(pos));
  }
  case 'u': {
    return new UserFactor(tmpchar, ScanInteger(pos));
  }
  case 'a': {
    return new AppFactor(tmpchar, ScanInteger(pos));
  }
  case 'A': {
    return new AppFanFactor(tmpchar, ScanInteger(pos));
  }
  case 'p': {
    return new PageFactor(tmpchar, ScanInteger(pos));
  }
  case 's': {
    return new HighSchoolFactor(tmpchar, ScanInteger(pos));
  }
  case 'g': {
    return new MiniGroupFactor(tmpchar, ScanInteger(pos));
  }
  case 'v': {
    return new FollowFactor(tmpchar, ScanInteger(pos));
  }
  case 'z': {
    return new XiaoZuFactor(tmpchar, ScanInteger(pos));
  }
  case 'x': {
    return new MiniSiteFactor(tmpchar, ScanInteger(pos));
  }
  case 'c': {
    return new CheWenFactor(tmpchar, ScanInteger(pos));
  }
  case 'j': {
    return new GuangJieFactor(tmpchar, ScanInteger(pos));
  }
  //shilong.li 2012-07-23
  //case 'J': {
    //ID is long type  
//    size_t start = expr_str_.find("(", pos);
//    size_t end = expr_str_.find(")", pos);
//    if (start > end) {
//      throw logic_error("Parser::ScanInteger() --> start larger than end");
//    }
//    pos = end + 1; 
//    string longstr = expr_str_.substr(start + 1, end - start - 1);
//    long value = boost::lexical_cast<long>(longstr);
    //return new GuangJieTopicFactor(tmpchar, value);
    //return new GuangJieTopicFactor(tmpchar, ScanLong(pos));
 // }
  case 't': {
    return new TagFactor(tmpchar, ScanInteger(pos));
  }
  //shilong.li 2012-07-23
// case 'b': {
//   return new AlbumFactor(tmpchar, ScanLong(pos));
// }
  default:
    throw logic_error("Parser::ScanFactor() --> wrong factor type = " + tmpchar);
    return NULL;
  }
}

Ice::Long Parser::ScanLong(size_t & pos) {//扫描因子括弧内的数字
  size_t start = expr_str_.find("(", pos);
  size_t end = expr_str_.find(")", pos);
  if (start > end) {
    throw logic_error("Parser::ScanInteger() --> start larger than end");
  }
  pos = end + 1;
  string intstr = expr_str_.substr(start + 1, end - start - 1);
  Ice::Long value = boost::lexical_cast<Ice::Long>(intstr);
  /*  MCE_DEBUG("Parser::ScanInteger() --> pos = " << pos << ", integer value = " << value);*/
  return value;
}

int Parser::ScanInteger(size_t & pos) {//扫描因子括弧内的数字
  size_t start = expr_str_.find("(", pos);
  size_t end = expr_str_.find(")", pos);
  if (start > end) {
    throw logic_error("Parser::ScanInteger() --> start larger than end");
  }
  pos = end + 1;
  string intstr = expr_str_.substr(start + 1, end - start - 1);
  int value = boost::lexical_cast<int>(intstr);
  /*  MCE_DEBUG("Parser::ScanInteger() --> pos = " << pos << ", integer value = " << value);*/
  return value;
}

void Parser::PrintIds() {
  MCE_DEBUG( "ids_'s size is :" << expr_ids_.size() );
  for (set<int>::iterator itr = expr_ids_.begin(); itr != expr_ids_.end(); ++itr) {
    MCE_DEBUG( *itr << " ");
  }
}

//--------------------------------------------------------------------------
void AppFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AppFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" AppFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }

  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["user_id"]);
  }
}

void AppFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND user_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AppFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" AppFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["user_id"]);
  }

}

void AppFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND user_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AppFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" AppFactor::Plus --> error:" << e.what());
  }
  MCE_DEBUG("AppFactor::And");
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["user_id"]);
  }

}

string AppFactor::GenerateSQL() { //app
  ostringstream tablestream;
  tablestream << "app_user_" << (param_id_ % 100);
  table_ = tablestream.str();
  ostringstream oss;
  oss << "SELECT user_id FROM " << table_ << " WHERE app_id = " << param_id_ << " ";
  string tmpstr = oss.str();
  return tmpstr;

}
//--------------------------------------------------------------------------
void AppFanFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AppFanFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" AppFanFactor::Plus --> error:" << e.what());
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["user_id"]);
  }
}

void AppFanFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND user_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AppFanFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" AppFanFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["user_id"]);
  }

}

void AppFanFactor::And(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND user_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" AppFanFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" AppFanFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["user_id"]);
  }

}

string AppFanFactor::GenerateSQL() { //app fans
  ostringstream tab;
  tab << "app_fans" << "_" << (param_id_ % 100);
  table_ = tab.str();
  ostringstream sql;
  sql << "SELECT user_id FROM " << table_ << " WHERE app_id=" << param_id_ << " ";
  string tmpstr = sql.str();
  return tmpstr;
}
//--------------------------------------------------------------------------

void PageFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e.what());
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["user_id"]);
  }
}

void PageFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND user_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["user_id"]);
  }

}

void PageFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND user_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["user_id"]);
  }

}

string PageFactor::GenerateSQL() {

  ServiceI &service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  instance_ = props->getPropertyWithDefault("Service.Database.PageFans",
      "page_fans");
  table_ = props->getPropertyWithDefault("Service.table.PageFans", "page_fans");

  ostringstream pageFansTab;
  pageFansTab << table_ << "_" << (param_id_ % 100);
  table_ = pageFansTab.str();
  ostringstream sql;
  sql << "SELECT user_id FROM " << pageFansTab.str() << " WHERE page_id="
      << param_id_ << " AND state=0";
  string tmpstr = sql.str();
  //MCE_DEBUG("PageFactor::GenerateSQL --> the sql head = " << tmpstr);
  return tmpstr;

}
//--------------------------------------------------------------------------
void FollowFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e.what());
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["follower_id"]);
  }
}

void FollowFactor::Minus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND follower_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["follower_id"]);
  }

}

void FollowFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND follower_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" PageFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["follower_id"]);
  }

}

string FollowFactor::GenerateSQL() {

  ServiceI &service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  instance_ = props->getPropertyWithDefault("Service.Database.VFollows",
      "page");
  table_ = props->getPropertyWithDefault("Service.table.VFollows", "v_follow_list");

  //ostringstream pageFansTab;
  //pageFansTab << table_ << "_" << (param_id_ % 100);
  //table_ = pageFansTab.str();
  ostringstream sql;
  sql << "SELECT follower_id FROM " << table_ << " WHERE v_id="
    << param_id_;
  string tmpstr = sql.str();
  //MCE_DEBUG("PageFactor::GenerateSQL --> the sql head = " << tmpstr);
  return tmpstr;

}
//--------------------------------------------------------------------------


void FriendFactor::Plus(set<int> & result) {
  //string head = GenerateSQL();
  //Statement sql;
  //sql << head;
  //mysqlpp::StoreQueryResult res;
  int count = 0;
  int begin = 0;
  int limit = 5000;
  MyUtil::IntSeq friends;
  do {
    try {
      //res = QueryRunner(instance_, CDbRServer, table_).store(sql);
      friends.clear();
      friends = adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(param_id_,begin,limit);
    } catch (Ice::Exception& e) {
      MCE_WARN(" FriendFactor::Plus --> error:" << e);
    } catch (std::exception& e) {
      MCE_WARN(" FriendFactor::Plus --> error:" << e.what());
    } catch (...) {
      MCE_WARN(" FriendFactor::Plis --> unkown error");
    }
    
    for (size_t i = 0; i < friends.size(); ++i) {
      result.insert(friends[i]);
    }
    MCE_INFO("FriendFactor::Plus --> uid:"<<param_id_<<" conut:"<<count<<" begin:"<<begin<<" friends size:"<<friends.size()<<" result size:"<<result.size());
    count++;
    begin += limit;
  } while(friends.size() >= limit );
}

void FriendFactor::Minus(set<int> & result) {

  /*string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND guest IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;*/

  int count = 0;
  int begin = 0;
  int limit = 5000;
  MyUtil::IntSeq friends;
  do {
    try {
      //  res = QueryRunner(instance_, CDbRServer, table_).store(sql);
      friends.clear();
      friends = adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(param_id_,begin,limit);
    } catch (Ice::Exception& e) {
      MCE_WARN(" FriendFactor::Minus --> error:" << e);
    } catch (std::exception& e) {
      MCE_WARN(" FriendFactor::Minus --> error:" << e.what());
    } catch (...) {
      MCE_WARN(" FriendFactor::Minus --> unkown error");
    }
    for (size_t i = 0; i < friends.size(); ++i) {
      if (result.find( friends[i]) == result.end()) 
        continue;
      result.erase(friends[i]);
    }
    if (result.empty()) {
      MCE_INFO("FriendFactor::Minus --> result has EMPTY so break loop;");
      break;
    }
    MCE_INFO("FriendFactor::Minus --> uid:"<<param_id_<<" conut:"<<count<<" begin:"<<begin<<" friends size:"<<friends.size()<<" result size:"<<result.size());
    count++;
    begin += limit;
  }while( friends.size() >= limit);

}

void FriendFactor::And(set<int> & result) {
  /*
  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND guest IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;*/
  int count = 0;
  int begin = 0;
  int limit = 5000;
  MyUtil::IntSeq friends;
  set<int> and_result;
  do {
    try {
      //res = QueryRunner(instance_, CDbRServer, table_).store(sql);
      friends.clear();
      friends = adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(param_id_,begin,limit);
    } catch (Ice::Exception& e) {
      MCE_WARN(" FriendFactor::And --> error:" << e);
    } catch (std::exception& e) {
      MCE_WARN(" FriendFactor::And --> error:" << e.what());
    } catch(...) {
      MCE_INFO(" FriendFactor::And --> unkown error");
    }
    for (size_t i = 0; i < friends.size(); ++i) {
      if (result.find(friends[i]) != result.end() ){
        and_result.insert(friends[i]);
      }
    }
   if (and_result.size() == result.size() ) {
     MCE_INFO("FriendFactor::And --> result size has FULL so break");
     break;
   } 
    MCE_INFO("FriendFactor::And --> uid:"<<param_id_<<" conut:"<<count<<" begin:"<<begin<<" friends size:"<<friends.size()<<"and_result size:"<<and_result.size()<<" result size:"<<result.size());
    count++;
    begin += limit; 
  }while(friends.size() >= limit);
  result.swap(and_result);
}

string FriendFactor::GenerateSQL() {
  ServiceI &service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  instance_ = props->getPropertyWithDefault("Service.Database.Relation",
      "xfeed_relation");
  table_ = props->getPropertyWithDefault("Service.table.Relation", "relation_");
  ostringstream tablename;
  tablename << table_ << (param_id_ % 100); //表名
  ostringstream sql;
  sql << "SELECT guest FROM " << tablename.str() << " WHERE host=" << param_id_
    << " ";
  table_ = tablename.str();
  return sql.str();
}

//--------------------------------------------------------------------------


void HighSchoolFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" HighSchoolFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" HighSchoolFactor::Plus --> error:" << e.what());
  }

  ostringstream oss;
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["uid"]);
    oss << res.at(i)["uid"] << ",";
  }
  //MCE_DEBUG("FriendFactor::Plus --> result ids --> " << oss.str());
}

void HighSchoolFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND uid IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" HighSchoolFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" HighSchoolFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["uid"]);
  }
}

void HighSchoolFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND uid IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" HighSchoolFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" HighSchoolFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["uid"]);
  }

}

string HighSchoolFactor::GenerateSQL() {
  ServiceI &service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  instance_ = props->getPropertyWithDefault("Service.Database.HIGHSCHOOL",
      "high_school_page");
  table_ = props->getPropertyWithDefault("Service.Table.ASP_MEMBER",
      "asp_member");
  ostringstream sql;
  sql << "SELECT uid FROM " << table_ << " WHERE pid=" << param_id_
    << "  AND status=0 AND stage <= 30 ";
  return sql.str();
}


//--------------------------------------------------------------------------


void MiniGroupFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head ;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e.what());
  }

  ostringstream oss;
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["member_id"]);
    //    oss << res.at(i)["uid"] << ",";
  }
  //MCE_DEBUG("FriendFactor::Plus --> result ids --> " << oss.str());
}

void MiniGroupFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND member_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["member_id"]);
  }
}

void MiniGroupFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND member_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["member_id"]);
  }

}

string MiniGroupFactor::GenerateSQL() {
  instance_ = "minigroup";//TODO
  ostringstream tmp;
  tmp << "minigroup_member_" <<  param_id_ % 100;
  table_ = tmp.str();
  ostringstream sql;
  sql << "SELECT member_id FROM " << table_ << " WHERE minigroup_id = " << param_id_ << " and access_id in (1,2,3) " ;
  return sql.str();

}
//--------------------------------------------------------------------------


void UserFactor::Plus(set<int> & result) {
  int intId = (int)param_id_;
  result.insert(intId);
}

void UserFactor::Minus(set<int> & result) {
  int intId = (int)param_id_;
  result.erase(intId);
}

void UserFactor::And(set<int> & result) {
  int intId = (int)param_id_;

  if (result.find(intId) != result.end()) {
    result.clear();
    result.insert(intId);
  } else {
    result.clear();
  }
}

//-------------------------------------------------------------------------
void XiaoZuFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head ;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e.what());
  }

  ostringstream oss;
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["member_id"]);
    //    oss << res.at(i)["uid"] << ",";
  }
  //MCE_DEBUG("FriendFactor::Plus --> result ids --> " << oss.str());
}

void XiaoZuFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND member_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["member_id"]);
  }
}

void XiaoZuFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND member_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniGroupFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["member_id"]);
  }

}

string XiaoZuFactor::GenerateSQL() {
  instance_ = "forum";
  ostringstream tmp;
  tmp << "forum_member_" << param_id_%100;
  table_ = tmp.str();
  ostringstream sql;
  sql << "SELECT member_id FROM " << table_ << " WHERE forum_id=" << param_id_ << " and access_id in (1,2,3) " ;
  return sql.str();

}
//---------------------------------------------------------------
void MiniSiteFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head ;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e.what());
  }

  ostringstream oss;
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["follower_id"]);
    //    oss << res.at(i)["uid"] << ",";
  }
  //MCE_DEBUG("FriendFactor::Plus --> result ids --> " << oss.str());
}

void MiniSiteFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND follower_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["follower_id"]);
  }
}

void MiniSiteFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND follower_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["follower_id"]);
  }

}

string MiniSiteFactor::GenerateSQL() {
  instance_ = "minisite_site_follower";
  ostringstream table_oss;
  table_oss <<"minisite_site_follower_"<<param_id_ % 100; 
  table_ = table_oss.str();
  ostringstream sql;
  sql << "SELECT follower_id FROM "<<table_<< " WHERE site_id=" << param_id_ ;
  return sql.str();
}
//---------------------------------------------------------------

void CheWenFactor::Plus(set<int> & result) {
  string head = GenerateSQL();
  Statement sql;
  sql << head ;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e.what());
  }

  ostringstream oss;
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["renren_id"]);
    //    oss << res.at(i)["uid"] << ",";
  }
  //MCE_DEBUG("FriendFactor::Plus --> result ids --> " << oss.str());
}

void CheWenFactor::Minus(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND renren_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e.what());
  }
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.erase(res.at(i)["renren_id"]);
  }
}

void CheWenFactor::And(set<int> & result) {

  string head = GenerateSQL();
  Statement sql;
  sql << head;

  sql << " AND renren_id IN (";
  set<int>::iterator itr = result.begin();
  sql << *itr;
  for (++itr; itr != result.end(); ++itr) {
    sql << "," << *itr;
  }
  sql << ")";

  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(instance_, CDbRServer, table_).store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e);
  } catch (std::exception& e) {
    MCE_WARN(" MiniSiteFactor::Plus --> error:" << e.what());
  }
  result.clear();
  if (!res) {
    MCE_WARN("StoryQueryResult wrong");
    return;
  }
  for (size_t i = 0; i < res.size(); ++i) {
    result.insert(res.at(i)["renren_id"]);
  }
}

string CheWenFactor::GenerateSQL() {
  instance_ = "chewen_user";
  table_ = "cw_followed_";
  ostringstream tablename;
  tablename << table_ << (param_id_ % 10); //表名
  ostringstream sql;
  sql << "SELECT renren_id FROM " << tablename.str() << " WHERE map_renren_id=" << param_id_;
  return sql.str();
}

void TagFactor::Plus(set<int> & result) {
  mysqlpp::StoreQueryResult res;
  Statement sql;
  string head;
  ostringstream sql_tail;
  struct timeval t_begin,t_end;
  long t_cost;
  int batch_size = 10000;
  int i_count = 0;
  int cur_max_id = 0;

  head = GenerateSQL();
  gettimeofday(&t_begin,NULL);//start time
  do {
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);
    sql_tail <<" AND follower_id > "<<cur_max_id<<" ORDER BY follower_id LIMIT " << batch_size;
    sql<<head<<sql_tail.str();
    try {
      res = QueryRunner(instance_, CDbRServer, table_).store(sql);
    } catch (Ice::Exception& e) {
       MCE_WARN(" TagFactor::Plus --> error:" << e);
    } catch (std::exception& e) {
       MCE_WARN(" TagFactor::Plus --> error:" << e.what());
    }
    for (size_t i = 0; i < res.size(); ++i) {
      int id = res.at(i)["follower_id"];
      result.insert(id);
      if (cur_max_id < id) {
        cur_max_id = id;
      }
    }
    i_count++;
    sql_tail.str("");
    sql.clear();
    gettimeofday(&t2,NULL);
    float tc = (t2.tv_sec - t1.tv_sec)*1000 + (t2.tv_usec - t1.tv_usec)*1.0 / (1000*1.0);
    MCE_DEBUG("TagFactor::Plus --> cur_max_id:"<<cur_max_id<<" i_count:"<<i_count<<" time cost:"<<tc<<" ms");
  } while (res.size() >=batch_size);

  gettimeofday(&t_end,NULL);//end time
  t_cost = ( t_end.tv_sec - t_begin.tv_sec )*1000 + ( t_end.tv_usec - t_begin.tv_usec )/1000;

  MCE_INFO("TagFactor::Plus -->result size:"<<result.size()<<" time cost:"<<t_cost<<"ms");
}
void TagFactor::Minus(set<int> & result) {
  mysqlpp::StoreQueryResult res;
  Statement sql;
  string head;
  ostringstream sql_tail;
  struct timeval t_begin,t_end;
  long t_cost;
  int batch_size = 10000;
  int i_count = 0;
  int cur_max_id = 0;

  head = GenerateSQL();
  gettimeofday(&t_begin,NULL);//start time
  do {
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);
    sql_tail <<" AND follower_id >"<<cur_max_id<<" ORDER BY follower_id LIMIT "<< batch_size;
    sql<<head<<sql_tail.str();
    try {
      res = QueryRunner(instance_, CDbRServer, table_).store(sql);
    } catch (Ice::Exception& e) {
      MCE_WARN(" MiniSiteFactor::Minus --> error:" << e);
    }  catch (std::exception& e) {
      MCE_WARN(" MiniSiteFactor::Minus --> error:" << e.what());
    }
    for (size_t i = 0; i < res.size(); ++i) {
      int id = res.at(i)["follower_id"];
      if (result.find(id) != result.end()) {
        result.erase(id);
      }
      if (cur_max_id < id) {
        cur_max_id = id;
      }
    }
    i_count++;
    sql_tail.str("");
    sql.clear();
    gettimeofday(&t2,NULL);
    float tc = (t2.tv_sec - t1.tv_sec)*1000 + (t2.tv_usec - t1.tv_usec)*1.0 / (1000*1.0);
    MCE_DEBUG("TagFactor::Minus --> cur_max_id:"<<cur_max_id<<" i_count:"<<i_count<<" time cost:"<<tc<<" ms");
  } while (res.size() >=batch_size);

  gettimeofday(&t_end,NULL);//end time
  t_cost = ( t_end.tv_sec - t_begin.tv_sec )*1000 + ( t_end.tv_usec - t_begin.tv_usec )/1000;

  MCE_INFO("TagFactor::Minus -->result size:"<<result.size()<<" time cost:"<<t_cost<<"ms");
}
void TagFactor::And(set<int> & result) {
  mysqlpp::StoreQueryResult res;
  set<int> res_swap;
  Statement sql;
  string head;
  ostringstream sql_tail;
  struct timeval t_begin,t_end;
  long t_cost;
  int batch_size = 10000;
  int i_count = 0;
  int cur_max_id = 0;

  head = GenerateSQL();
  gettimeofday(&t_begin,NULL);//start time
  do {
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);
    sql_tail <<" AND  follower_id >"<<cur_max_id<<" ORDER BY follower_id  LIMIT "<< batch_size;
    sql<<head<<sql_tail.str();
    try {
      res = QueryRunner(instance_, CDbRServer, table_).store(sql);
    } catch (Ice::Exception& e) {
      MCE_WARN(" TagFactor::And --> error:" << e);
    }  catch (std::exception& e) {
      MCE_WARN(" TagFactor::And --> error:" << e.what());
    }
    for (size_t i = 0; i < res.size(); ++i) {
      int id = res.at(i)["follower_id"];
      if (result.find(id) != result.end()) {
        res_swap.insert(id);
      }
      if (cur_max_id < id) {
        cur_max_id = id;
      }
    }

    i_count++;
    sql_tail.str("");
    sql.clear();
    gettimeofday(&t2,NULL);
    float tc = (t2.tv_sec - t1.tv_sec)*1000 + (t2.tv_usec - t1.tv_usec)*1.0 / (1000*1.0);
    MCE_DEBUG("TagFactor::And --> cur_max_id:"<<cur_max_id<<" i_count:"<<i_count<<" time cost:"<<tc<<" ms");
  } while (res.size() >=batch_size);
  gettimeofday(&t_end,NULL);//end time
  result.swap(res_swap);

  t_cost = ( t_end.tv_sec - t_begin.tv_sec )*1000 + ( t_end.tv_usec - t_begin.tv_usec )/1000;
  MCE_INFO("TagFactor::And -->result size:"<<result.size()<<" time cost:"<<t_cost<<" ms");
}

string TagFactor::GenerateSQL() {
  instance_ = "minisite";
  table_ =  "tag_follower";
  ostringstream tablename;
  tablename<< table_ ;
  ostringstream sql;
  sql << "SELECT follower_id FROM " << tablename.str() << " WHERE tag_id=" << param_id_ ;
  return sql.str();
}

void GuangJieFactor::Plus(set<int> & result) {
  struct timeval begin,end;
  vector<int64_t> followers;
  int64_t uid = param_id_;
  int64_t index   = 0;
  int limit   = 10000;
  int count   = 0;
  gettimeofday(&begin,NULL);
  do {
    try{
      followers.clear();
      xce::ad::UserCacheClientPlus::instance().GetFansByIndex(followers,uid,index,limit);
      if (followers.size() > 0) {
        int len = followers.size();
        index = followers[len-1];
      } 
      count++;
    } catch(std::exception& e) {
      MCE_WARN("GuangJieFator::plus -->   error: "<<e.what()<<" followers size:"<<followers.size()<<" uid:"<<uid<<" limit:"<<limit);
    } catch(...) {
      MCE_WARN("GuangJieFator::plus -->   error unkown  followers size:"<<followers.size()<<" uid:"<<uid<<" limit:"<<limit);
    }
    for (vector<int64_t>::iterator i = followers.begin(); i != followers.end(); ++i) {
      result.insert(*i);
    }
    MCE_INFO("GuangJieFactor::Plus --> uid:"<<uid<<" count:"<<count<<" index:"<<index<<" followers size:"<<followers.size()<<" result size:"<<result.size());
    if(count >=100) {
      MCE_WARN("GuangJieFactor::Plus --> Exceed the limit count:"<<count<<" uid:"<<uid);
      break;
    }
  }while(followers.size() >= limit);

  gettimeofday(&end,NULL);
  float ts = (end.tv_sec - begin.tv_sec)*1000 + (end.tv_usec - begin.tv_usec)/1000;
  MCE_INFO("GuangJieFactor::Plus --> OK uid:"<<uid<<" result size:"<<result.size()<<" time cost:"<<ts<<" ms");

}

void GuangJieFactor::Minus(set<int> & result) {
  struct timeval begin,end;
  vector<int64_t> followers;
  int64_t uid = param_id_;
  int64_t index = 0;
  int limit   = 10000;
  int count   = 0;
  gettimeofday(&begin,NULL);
  do {
    try{
      followers.clear();
      xce::ad::UserCacheClientPlus::instance().GetFansByIndex(followers,uid,index,limit);
      if (followers.size() > 0 ) {
        int len = followers.size();
        index = followers[len -1];
      }
      count++;
    } catch(std::exception& e) {
      MCE_WARN(" GuangJieFactor::Minus --> error:" << e.what());
    } catch(...) {
      MCE_WARN("GuangJieFactor::Minus -->error ");
    }
    for (vector<int64_t>::iterator i = followers.begin(); i != followers.end(); ++i) {
      if ( result.find(*i) == result.end())
        continue;
      result.erase(*i);
    }
    MCE_INFO("GuangJieFactor::Minus --> uid:"<<uid<<" count:"<<count<<" index:"<<index<<" followers size:"<<followers.size()<<" result size:"<<result.size());
    // 如果resualt已经为空,没必要继续执行
    if ( result.empty() ) {
      MCE_INFO("GuangJieFactor::Minus --> result has EMPTY! uid:"<<uid<<" count:"<<count<<" index:"<<index);
      break;
    }
    if(count >=100) {
      MCE_WARN("GuangJieFactor::Plus --> Exceed the limit count:"<<count<<" uid:"<<uid);
      break;
    }
  } while(followers.size() >= limit);
  gettimeofday(&end,NULL);
  float ts = (end.tv_sec - begin.tv_sec)*1000 + (end.tv_usec - begin.tv_usec)/1000;
  MCE_INFO("GuangJieFactor::Minus --> OK uid:"<<uid<<" result size:"<<result.size()<<" time cost:"<<ts<<" ms");

}

void GuangJieFactor::And(set<int> & result) {
  struct timeval begin,end;
  vector<int64_t> followers;
  set<int> and_result;
  int64_t uid = param_id_;
  int64_t index = 0;
  int limit   = 10000;
  int count   = 0;
  gettimeofday(&begin,NULL);
  do {
    try{
      followers.clear();
      xce::ad::UserCacheClientPlus::instance().GetFansByIndex(followers,uid,index,limit);
      if (followers.size() > 0 ) {
        int len = followers.size();
        index = followers[len - 1];
      }
      count++;
    } catch(std::exception& e) {
      MCE_WARN(" GuangJieFactor::And --> error:" << e.what());
    } catch(...) {
      MCE_WARN("GuangJieFactor::And -->error ");
    }
    for (vector<int64_t>::iterator i = followers.begin(); i != followers.end(); ++i) {
      set<int>::iterator sit = result.find(*i);
      if (sit != result.end()){
        and_result.insert(*i);
      }
    }
    MCE_INFO("GuangJieFactor::And --> uid:"<<uid<<" count:"<<count<<" index:"<<index<<" followers size:"<<followers.size()<<" result size:"<<result.size());
    if (and_result.size() == result.size()) {
      MCE_INFO("GuangJieFactor::And --> and_result has FULL! uid:"<<uid<<" count:"<<count<<" index"<<index);
      break;
    }
    if(count >=100) {
      MCE_WARN("GuangJieFactor::Plus --> Exceed the limit count:"<<count<<" uid:"<<uid);
      break;
    }
  } while(followers.size() >= limit);
  result.swap(and_result);
  gettimeofday(&end,NULL);
  float ts = (end.tv_sec - begin.tv_sec)*1000 + (end.tv_usec - begin.tv_usec)/1000;
  MCE_INFO("GuangJieFactor::And --> OK uid:"<<uid<<" result size:"<<result.size()<<" time cost:"<<ts<<" ms");

}
//shilong.li 2012-07-23
/*
   void GuangJieTopicFactor::Plus(set<int> & result) {
   vector<int64_t> followers;
   int64_t uid = param_id_;
//int64_t uid = param_idL_;
int limit   = 2000;
try{
xce::ad::UserCacheClientPlus::instance().GetTopicFollower(followers,uid,limit);
} catch(std::exception& e) {
MCE_WARN(" GuangJieTopicFactor::plus --> error:" << e.what());
} catch(...) {
MCE_WARN("GuangJieTopicFactor::plus -->error ");
}

if (followers.empty()) {
MCE_INFO("GuangJieTopicFactor::Plus --> GetTopicFollower return empty");
return;
}
MCE_INFO("GuangJieTopicFactor::Plus -->GetTopicFollower return size:"<<followers.size());
for (vector<int64_t>::iterator i = followers.begin(); i != followers.end(); ++i) {
result.insert(*i);
}
MCE_INFO("GuangJieTopicFactor -->Plus result size;"<<result.size());
}

void GuangJieTopicFactor::Minus(set<int> & result) {
vector<int64_t> followers;
int64_t uid = param_id_;
//int64_t uid = param_idL_;
int limit   = 2000;
try{
xce::ad::UserCacheClientPlus::instance().GetTopicFollower(followers,uid,limit);
} catch(std::exception& e) {
MCE_WARN(" GuangJieTopicFactor::Minus --> error:" << e.what());
} catch(...) {
MCE_WARN("GuangJieTopicFactor::Minus -->error ");
}

if (followers.empty()) {
MCE_INFO("GuangJieTopicFactor::Minus -->GetTopicFollower return empty");
return;
}
MCE_INFO("GuangJieTopicFactor::Minus -->GetTopicFollower return size:"<<followers.size());
for (vector<int64_t>::iterator i = followers.begin(); i != followers.end(); ++i) {
if ( result.find(*i) == result.end())
continue;
result.erase(*i);
}
MCE_INFO("GuangJieTopicFactor::Minus --> result size:"<<result.size());
}

void GuangJieTopicFactor::And(set<int> & result) {
vector<int64_t> followers;
set<int> and_result;
int64_t uid = param_id_;
//int64_t uid = param_idL_;
int limit   = 2000;
try{
xce::ad::UserCacheClientPlus::instance().GetTopicFollower(followers,uid,limit);
} catch(std::exception& e) {
MCE_WARN(" GuangJieTopicFactor::And --> error:" << e.what());
} catch(...) {
MCE_WARN("GuangJieTopicFactor::And -->error ");
}

if (followers.empty()) {
MCE_INFO("GuangJieTopicFactor::And -->GetTopicFollower return empty");
return;
}

MCE_INFO("GuangJieTopicFactor::And -->GetTopicFollower return size:"<<followers.size());
for (vector<int64_t>::iterator i = followers.begin(); i != followers.end(); ++i) {
  set<int>::iterator sit = result.find(*i);
  if (sit != result.end()){
    and_result.insert(*i);
  }
}
result.swap(and_result);
MCE_INFO("GuangJieTopicFactor::And -->And reuslt size:"<< result.size());
}


void AlbumFactor::Plus(set<int> & result) {
  vector<int64_t> fans;
  int64_t album_id = param_id_;
  int limit   = 20000;
  try{
    xce::ad::UserCacheClientPlus::instance().GetAlbumFans(fans, album_id,  0, limit);
  } catch(std::exception& e) {
    MCE_WARN(" AlbumFactor::plus --> error:" << e.what());
  } catch(...) {
    MCE_WARN("AlbumFactor::plus -->error ");
  }

  if (fans.empty()) {
    MCE_WARN("AlbumFactor::Plus -->GetFans return empty");
    return;
  }
  MCE_DEBUG("AlbumFactor::Plus -->GetFans size:"<<fans.size() << " album_id:" << album_id);
  for (vector<int64_t>::iterator i = fans.begin(); i != fans.end(); ++i) {
    result.insert(*i);
  }
  MCE_INFO("AlbumFactor -->Plus result size;"<<result.size());
}

void AlbumFactor::Minus(set<int> & result) {
  vector<int64_t> fans;
  int64_t album_id = param_id_;
  int limit   = 20000;
  try{
    xce::ad::UserCacheClientPlus::instance().GetAlbumFans(fans, album_id,  0, limit);
  } catch(std::exception& e) {
    MCE_WARN(" AlbumFactor::Minus --> error:" << e.what());
  } catch(...) {
    MCE_WARN("AlbumFactor::Minus -->error ");
  }

  if (fans.empty()) {
    MCE_WARN("AlbumFactor::Minus -->GetFans return empty");
    return;
  }
  MCE_DEBUG("AlbumFactor::Minus -->GetFans size:"<<fans.size()<< " album_id:" << album_id);
  for (vector<int64_t>::iterator i = fans.begin(); i != fans.end(); ++i) {
    result.erase(*i);
  }
  MCE_INFO("AlbumFactor -->Minus result size:"<<result.size());
}

void AlbumFactor::And(set<int> & result) {
  vector<int64_t> fans;
  set<int> and_result;
  int64_t album_id = param_id_;
  int limit   = 20000;
  try{
    xce::ad::UserCacheClientPlus::instance().GetAlbumFans(fans, album_id,  0, limit);
  } catch(std::exception& e) {
    MCE_WARN(" AlbumFactor::And --> error:" << e.what());
  } catch(...) {
    MCE_WARN("AlbumFactor::And -->error ");
  }

  if (fans.empty()) {
    MCE_WARN("AlbumFactor::And -->GetFans return empty");
    return;
  }

  MCE_DEBUG("AlbumFactor::And -->GetFans size:"<<fans.size()<< " album_id:" << album_id);
  for (vector<int64_t>::iterator i = fans.begin(); i != fans.end(); ++i) {
    set<int>::iterator sit = result.find(*i);
    if (sit != result.end()){
      and_result.insert(*i);
    }
  }
  result.swap(and_result);
  MCE_INFO("AlbumFactor::And -->And Reusalt size:"<< result.size());
}*/
