#include "PingComputorI.h" 
#include <numeric>
#include <cmath>
//#include <math.h>
#include <boost/lexical_cast.hpp>
#include "client/linux/handler/exception_handler.h"
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "LogWrapper.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "QueryRunner.h"

using namespace com::xiaonei::xce;
using namespace xce::ad;
using namespace std;
using namespace boost;
using namespace MyUtil;

void MyUtil::initialize() {
  MCE_INFO("PingComputor --> start");
  ServiceI& service = ServiceI::instance();
  string db_instance = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance", "db_adwr");
  MCE_INFO("PingComputor --> db_instance=" << db_instance);
  PingComputorI::instance().Init(db_instance);
  
  service.getAdapter()->add(&PingComputorI::instance(), service.createIdentity("PC",""));

  TaskManager::instance().schedule(new ComputeTimer());
  MCE_INFO("PingComputor --> stared");
}

//设置数据源
void PingComputorI::Init(const string& DbInstance){
  db_source_ = DbInstance;
  MyUtil::Date date(time(NULL));
  begin_time_ = date.toDateTime("%Y%m%d%H%M%S");
  interval_ = 60 * 24;
}

void PingComputorI::Reset(const string& time, const int interval, const Ice::Current&) {
  begin_time_ = time;
  interval_ = interval;
  MCE_INFO("PingComputorI::Reset --> begin_time_:" << begin_time_ << ", interval_:" << interval_ );
}

//对外接口，提供统计服务方法的某一数值，1-nn, 2-nz, 3-avg, 4-stdev
double PingComputorI::StatServerMethod(const string& serverName, const string& methodName, 
    const string& beginTime, const string& endTime, const int type, const Ice::Current&){
  Statement sql;
  sql << " SELECT server_name, method_name, responsetime, require_num, updatetime "
    << " FROM server_ping_info "
    << " WHERE updatetime >= " << beginTime << " AND updatetime <= " << endTime
    << " AND server_name = \'" << serverName << "\' AND method_name = \'" << methodName << "\'";
  multiset<double> mset;
  try {
    mysqlpp::StoreQueryResult result = QueryRunner(db_source_, CDbRServer, "server_ping_info").store(sql);
    if (result && result.num_rows() > 0) {
      for (size_t i = 0; i < result.num_rows(); ++i) {
        mysqlpp::Row row = result.at(i);
        double responsetime = mysql_double(row["responsetime"]);
        mset.insert(responsetime);
      }
    }
  }  catch (Ice::Exception& e) {
    MCE_WARN("PingComputorI::StatServerMethod :" << e);
  } catch (std::exception& e) {
    MCE_WARN("PingComputorI::StatServerMethod :" << e.what());
  }

  if (mset.empty()) {
    return 0;
  }
  double result = 0;
  switch (type) {
    case NN:
      result = ComputeNN(mset); 
      break;
    case NZ:
      result = ComputeNZ(mset); 
      break;
    case AVG:
      result = ComputeAvg(mset); 
      break;
    case STDEV:
      result = ComputeStdDev(mset);
      break;
    default:
      result = 0;
      break;
  }
  return result;
}

/*
 *Func:PingComputorI::Load
 *Desc:加载各服务方法的响应时间
 */
void PingComputorI::Load(const string& begin_time, const string& end_time, map<string, multiset<double> >& pool) {
  Statement sql;
  sql << " SELECT server_name, method_name, responsetime, require_num, updatetime "
      << " FROM server_ping_info "
      << " WHERE updatetime >= " << begin_time << " AND updatetime <= " << end_time;
  pool.clear();
  try {
    mysqlpp::StoreQueryResult result = QueryRunner(db_source_, CDbRServer, "server_ping_info").store(sql);
    if (result && result.num_rows() > 0) {
      for (size_t i = 0; i < result.num_rows(); ++i) {
        mysqlpp::Row row = result.at(i);
        string server_name = mysql_string(row["server_name"]);
        string method_name = mysql_string(row["method_name"]);
        double responsetime = mysql_double(row["responsetime"]);
        string key = server_name + "_" + method_name;
        map<string, multiset<double> >::iterator it = pool.find(key);
        if (it == pool.end()) {
          multiset<double> ms;
          ms.insert(responsetime);
          pool.insert(make_pair(key, ms));
        } else {
          it->second.insert(responsetime); 
        }
      }
    }
    MCE_INFO("PingComputorI::Load --> server_ping_info, " << begin_time << "-" << end_time 
        << ", data size:" << pool.size());
  } catch (Ice::Exception& e) {
    MCE_WARN("PingComputorI::Load :" << e);
  } catch (std::exception& e) {
    MCE_WARN("PingComputorI::Load :" << e.what());
  }
}

/*
 *Func:PingComputorI::ComputeNN
 *Desc:计算数据中的.99值
 */
double PingComputorI::ComputeNN(const multiset<double>& pool) {
  double index = pool.size() * 0.99;
  multiset<double>::iterator it = pool.begin();
  for (int i = 1; i < index; ++i, ++it);
  return *it;
}

/*
 *Func:PingComputorI::ComputeNZ
 *Desc:计算数据中的.90值
 */
double PingComputorI::ComputeNZ(const multiset<double>& pool) {
  double index = pool.size() * 0.9;
  multiset<double>::iterator it = pool.begin();
  for (int i = 1; i < index; ++i, ++it);
  return *it;
}

/*
 *Func:PingComputorI::ComputeAvg
 *Desc:计算数据中的平均值
 */
double PingComputorI::ComputeAvg(const multiset<double>& pool) {
  int size = pool.size();
  double sum = accumulate(pool.begin(), pool.end(), 0.0); 
  return sum/size;
}

/*
 *Func:PingComputorI::ComputeStdDev
 *Desc:计算数据的标准差
 */
double PingComputorI::ComputeStdDev(const multiset<double>& pool) {
  if (pool.empty()) {
    return 0;
  }
  double avg = ComputeAvg(pool);
  multiset<double>::iterator it = pool.begin();
  double sum = 0;
  for (; it != pool.end(); ++it) {
    sum += pow((*it - avg), 2);
  }
  return sqrt(sum/pool.size());
}

/*
 *Func:PingComputorI::Compute
 *Desc:统计服务方法的时间,供timer定期调用
 */
void PingComputorI::Compute() {
  //大于统计间隔，才会进行计算
  MyUtil::Date now(time(NULL));
  MyUtil::Date begin(begin_time_, "%Y%m%d%H%M%S");
  MCE_INFO("PingComputorI::Compute: begin_time_:" << begin_time_ << " interval(m):" << interval_);
  if ((now.toSeconds() - begin.toSeconds()) < interval_*60) {
    return;
  }
  
  map<string, multiset<double> > local_pool;
  Load(begin_time_, now.toDateTime("%Y%m%d%H%M%S"), local_pool);
  if (local_pool.empty()) {
    return;
  }
  Statement sql;
  sql << " INSERT INTO server_ping_stat(server_name, method_name, time_nn, time_nz, time_avg, time_std_deviation, begin_time, end_time) "
    << " VALUES ";
  map<string, multiset<double> >::iterator it = local_pool.begin();
  size_t count = 1;
  for (; it != local_pool.end(); ++it, ++count) {
    string key = it->first;
    int i = key.find('_');
    string server_name = key.substr(0, i);
    string method_name = key.substr(i+1);
    double nn = ComputeNN(it->second);
    double nz = ComputeNZ(it->second);
    double avg = ComputeAvg(it->second);
    double std_dev = ComputeStdDev(it->second);
    MCE_DEBUG("PingComputorI::Compute --> server_name:" << server_name << " method_name:" << method_name
        << " nn:" << nn << " nz:" << nz << " avg:" << avg << " std_dev:" << std_dev);
    sql << " (\'" << server_name << "\', \'" << method_name << "\', \'" << nn << "\', \'" << nz 
        << "\', \'" << avg << "\', \'" << std_dev << "\', " << begin_time_ << ", " << now.toDateTime("%Y%m%d%H%M%S")
        << ")";
    if (count != local_pool.size()) {
      sql << ",";
    }
  }
  try {
    QueryRunner(db_source_, CDbWServer, "server_ping_stat").store(sql);
  } catch (Ice::Exception& e) {
    MCE_WARN("PingComputorI::Compute: Ice Exception: " << e);
  } catch (std::exception& e) {
    MCE_WARN("PingComputorI::Compute: std exception: " << e.what());
  } catch (...) {
    MCE_WARN("PingComputorI::Compute: unknown exception");
  }
  //重置begin_time_
  begin_time_ = now.toDateTime("%Y%m%d%H%M%S");
}

void ComputeTimer::handle() {
  PingComputorI::instance().Compute();
  TaskManager::instance().schedule(new ComputeTimer());
}
