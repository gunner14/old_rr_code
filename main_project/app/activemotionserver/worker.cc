#include "worker.h"
#include "hadooplogger.h"
//#include <locallogger.h>
#include "socketlogger.h"
#include <ostream>

using namespace std;

/*****************************************************************************/

void WorkMessage::Run(){
	SetDataHelper::GetInstance().InvokeDelay(type_,time_,param_);
}

/*****************************************************************************/

void NotifyMessage::Run(){
  for( size_t i=0; i<datas_.size(); ++i ){
    if(datas_[i]->type_ == 1) {
      try {
        specialHandle(datas_[i]->time_, datas_[i]->param_); 
      } catch(...) {
        //TODO:nothing
      }
    } else {
      Json::Value value;
      Json::Reader reader;
      try {
        const std::string& paramJ = datas_[i]->param_;
        if(reader.parse(paramJ, value) && value.isObject()) {
//这样做的目的是为了仅仅捕获登录用户的行为；
          int id = boost::lexical_cast<int>(value.get(DjParamID, 0).asString() );
          std::string r_url = value.get(DjParamR, "NULL").asString();
          std::string u_url = value.get(DjParamU, "").asString();
          std::string t_url = value.get(DjParamT, "").asString();
          if(datas_[i]->type_ == 6) {
            std::string o_info = value.get(DjParamO, "NULL").asString();
            HADOOP_INFO(datas_[i]->time_ << " " << id << " " << datas_[i]->type_ << " " << r_url << " " << o_info);
          } else {
            HADOOP_INFO(datas_[i]->time_ << " " << id << " " << datas_[i]->type_ << " " << r_url);
          }
        } 
      } catch(...) {
        //TODO: nothing;
      }
    }
  }
}
void NotifyMessage::specialHandle(int time, const std::string& paramJ) {
  using namespace std;
  vector<string> params = NotifyMessage::splitParams(paramJ, '{', '}');
  Json::Value value;
  for (vector<string>::iterator iter = params.begin(); iter!= params.end(); iter++) {
    Json::Value tempValue;
    Json::Reader tempReader;
    Json::Value::iterator jsonIter;
    Json::Value::Members members;
    if (tempReader.parse(*iter, tempValue) && tempValue.isObject()) {
      members = tempValue.getMemberNames();
      for (Json::Value::Members::iterator memIter = members.begin(); memIter !=members.end(); memIter++) {
        value[*memIter] = tempValue[*memIter];
      } 
    }
  }
  if (value.isObject()) {
    //int id = boost::lexical_cast<int>(value.get(DjParamID, 0).asString() );
    std::string id = value.get(DjParamID, "").asString();
    if (id.empty()) {
      id = "0";
    }
    std::string r_url = value.get(DjParamR, "NULL").asString();
    std::string u_url = value.get(DjParamU, "").asString();
    std::string t_url = value.get(DjParamT, "").asString();
    std::string clientIP = value.get(DjParamIP, "NULL").asString();
    std::string userAgent = value.get(DjParamUserAgent, "NULL").asString();
    int xpos = value.get(DjParamX, 0).asInt();
    int ypos = value.get(DjParamY, 0).asInt();
    if(u_url == "") {
      if (id != "0") {
        HADOOP_INFO(time << " " << id << " 1 " << r_url);
      }
      HADOOPCLICK_INFO(time << "\t" << id <<"\t1\t" << r_url << "\t" << xpos << "\t" << ypos << "\t" << clientIP << "\t" << userAgent);
    } else {
      if (id != "0") {
        HADOOP_INFO(time << " " << id << " 0 " << r_url << " " << u_url << " " << t_url);
      }
      HADOOPCLICK_INFO(time << "\t" << id <<"\t0\t" << r_url << "\t" << xpos << "\t" << ypos << "\t" << clientIP << "\t" << userAgent << "\t" << u_url << "\t" << t_url);
    }
  }
}

std::vector<std::string> NotifyMessage::splitParams(const std::string& param, const char splitB, const char splitE) {
  using namespace std;
  vector<string> result;
  string::size_type posB = 0, posE = 0;
  while ( (posB = param.find_first_of(splitB, posB)) != string::npos) {
    int stackSize = 0;
    if (posB > posE) {
      result.push_back(string(param, posE, posB));
      posE = posB;
    }
    if (posB >= param.size()) 
      break;
    else {
      stackSize++;
      while (stackSize != 0 && ++posE < param.size()) {
        if (param[posE] == splitB) {
          stackSize++;
        } else if (param[posE] == splitE) {
          stackSize--;
        }
      }
      result.push_back(string(param, posB, (++posE - posB) ));
      posB = posE;
    }
  }
  if (posB == string::npos && posE < param.size()) {
     result.push_back(string(param, posE, param.size()));
  }
  return result;
}

/*****************************************************************************/

static void InitSetDataHelper() {
	LOG(INFO) << "==> SetDataHelper inited !";
	//FCGI_INFO( "==> SetDataHelper inited !");
}

SetDataHelper& SetDataHelper::GetInstance() {
	static XCE_DECLARE_ONCE(once_guard_);
	static SetDataHelper sdh_;
	xce::XceOnceInit(&once_guard_, &InitSetDataHelper);
	return sdh_;
}

/*****************************************************************************/

void SetDataHelper::InvokeDelay( int type, int time, const std::string& param ){
	{
		boost::mutex::scoped_lock alock(mutex_);
		datas_.push_back( boost::shared_ptr<NotifyData>( new NotifyData(type,time,param) ) );
		if (datas_.size() >= MAX_DATA_SIZE ){
			LOG(WARNING) << "==> size: " << datas_.size() << " max_size_notify_now";
			//FCGI_WARN( "==> size: " << types_.size() << " max_size_notify_now");
			datas_full_.notify_all();
		}
	}
}
void SetDataHelper::InvokeNow( int type, int time, const std::string& param ){
	{
		boost::mutex::scoped_lock alock(mutex_);
		datas_.push_back( boost::shared_ptr<NotifyData>( new NotifyData(type,time,param) ) );
		datas_full_.notify_all();
	}
}

void SetDataHelper::Swap(std::vector<boost::shared_ptr<NotifyData> >& datas){
	{
		boost::mutex::scoped_lock alock(mutex_);
		if (datas_.size() < MAX_DATA_SIZE ){
			boost::system_time tAbsoluteTime = boost::get_system_time() + boost::posix_time::seconds(MAX_WAIT_TIME);
			datas_full_.timed_wait(alock,tAbsoluteTime );
		}
		datas_.swap( datas );
	}
}

void SetDataHelper::SetDataMethod::Run() {
	while (true){
		std::vector<boost::shared_ptr<NotifyData> > datas;

		helper_->Swap( datas );
		if( datas.empty() ){
			LOG(WARNING) << "==> notify_empty";
			//FCGI_WARN( "==> notify_empty");
			continue;
		}

		LOG(INFO) << "==> notify_size: " << datas.size();
		//FCGI_INFO( "==> notify_size: " << types.size());
		notify_thrdp_.Post( new NotifyMessage(datas) );
	}
}

