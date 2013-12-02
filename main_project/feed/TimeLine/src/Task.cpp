///////////////////////////////////////////////////
///
///
///   ChangeLog: 10/23/2012  change all xml format to <hour>json</hour>
//
///
/////////////////////////////////////////////////////
#include "preheader.h"
#include "Task.h"
#include "TimeLineFeedI.h"
#include "TimeLineInternal.h"
#include "pugixml_v1.hpp"

using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate;
using namespace xce::subtle;
using namespace xce::feed::timeline;
using namespace Ice;
using namespace pugi;


// stat log format (xml + json)
//
// <d2012123110>
// {
// "prop0":123,
// "prop1":123,
// "prop2":123,
// "prop3":123
// }
// </d2012123110>
//
void TimeLineStaticsTask::runTimerTask()
{
    TimeLineStatistics & stat =TimeLineFeedI::instance().stat_;
    int year,month,day,hour,min,sec;
    getDayTime(time(NULL),year,month,day,hour,min,sec);
    unsigned int temp_time=year*1000000;  temp_time += month*10000; temp_time += day*100; temp_time += hour;      
    if(stat.time_!=temp_time)//save
    {
        string _path;
        TimeLineFeedI::instance().stat_path(_path);
        if(!_path.empty())
        {
            //create file when needed.
            ofstream temp_f(_path.c_str(),ios::app);
            if(!temp_f.fail())
            {
                xml_document doc;
                stringstream ss;stringstream xml;
                ss<<"d"<<stat.time_;
                string time_name=ss.str();
		ss.clear();ss.str("");
                ss<<"{";
                for(int i=0;i<kStatMax;++i)
                {
                    ss<<"\n\""<<stat.names_[i]<<"\": "<<NoBarrier_Load(&(stat.values_[i]))<<",";
                }
                ss<<"\n}";
                xml_node node = doc.append_child(time_name.c_str());
                xml_node value_node;
		node.append_child(node_pcdata).set_value(ss.str().c_str());
                doc.save(xml);
                string temp=xml.str();
                size_t pos=temp.find('>');
                if(pos!=string::npos && pos < temp.size()-1)
                {
                    temp_f<<temp.substr(pos+1);
                    MCE_INFO("TimeLineStaticsTask::runTimerTask --> write: "<<temp.substr(pos+1));
                    temp_f.close();
                }
            } else {
                MCE_INFO("TimeLineStaticsTask::runTimerTask --> open file failed.file:"<<_path);
            }
        }
        stat.time_=temp_time;
    }
}
void LoadCacheByFileTask::handle()
{
    int update_size = 0;
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    fstream file(file_name.c_str());
    if( file.fail() ) {
        MCE_INFO("LoadCacheByFileTask::handle --> file error:"<<file_name);
    }
    MCE_DEBUG("LoadCacheByFileTask::handle --> quotient:"<<quotient<<" remainder:"<<remainder);
    string line;TimeLineObjectPtr ptr;
    int uid;stringstream ss;
    size_t nums=0;size_t load_nums=0;
    while( getline(file,line,'\n') )
    {
        if( update_size>=50 ) {
            update_size=0;
            IceUtil::Mutex::Lock lock(feedI.stat_.mutex_);
            ss.clear(); ss.str(""); ss<<"processed:"<<nums<<" loaded:"<<load_nums;
            feedI.stat_.table_[progress_name]=ss.str();
        }
        ss.clear(); ss.str(line); ss>>uid;
        if(quotient!=0) {
            if( (uid%quotient)!=remainder) {
                ++nums;
                continue;
            }
        }
        ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
        IceUtil::Mutex::Lock lock(ptr->mutex_);
        feedI.loadCache(ptr,uid);
        ++update_size;
        ++nums;
        ++load_nums;
    }
    {
        IceUtil::Mutex::Lock lock(feedI.stat_.mutex_);
        feedI.stat_.table_.erase("task");
    }
}
void LoadCacheTask::handle()
{
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    TimeLineObjectPtr ptr;
    for(vector<int>::iterator it=uids.begin();it!=uids.end();++it) 
    {
        ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,*it);
        IceUtil::Mutex::Lock lock(ptr->mutex_);
        feedI.loadCache(ptr,*it,0,0,true);
    }
}
