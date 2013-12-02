
#include <Ice/BuiltinSequences.h>
#include "src/client/client.h"
#include "xlog.h"
#include "src/client/xlog_appender.h"
#include "src/client/xlog_properties.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

using namespace std;

namespace xlog
{
    template<class P> string vector2String(vector<P> v);



    XLogAppender::XLogAppender(const vector<string>& categories, const char* properties_file_path):_categories(categories),_strlen(0){

        XLogProperties properties(properties_file_path);
        init(properties);
    }

    XLogAppender::XLogAppender(const vector<string>& categories,
    const bool is_udp_protocol, const int maxSendSize, int maxQueueSize, string& hosts):_categories(categories),_strlen(0)
    {
        XLogProperties properties(hosts, is_udp_protocol, maxSendSize, maxQueueSize);
        init(properties);
    }

    void XLogAppender::init(const XLogProperties& properties){
         vector<string> temp =properties.getHosts();
        _client = new Client(temp, properties.isUdpProtocol(), properties.getMaxQueueSize());
        cout << "xlog client configuration parameters: " << endl;
        cout << "hosts: " << xlog::vector2String(temp) << ", isUdpProtocol: "<< properties.isUdpProtocol() << ", maxSendSize:" << properties.getMaxSendSize() << ", maxQueueSize: "<< properties.getMaxQueueSize() << endl;
        _maxSendSize = properties.getMaxSendSize();
    }


    int XLogAppender::append(const char* msg, int const len){
        string str(msg, len);
        return append(str);
    }

    int XLogAppender::append(const std::string& msg)
    {
        _strlen += msg.length();
        ::IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_dataMutex);
        _logSeq.push_back(msg);
        if(_strlen >= _maxSendSize){
            xlog::slice::LogDataSeq logDataSeq;
            xlog::slice::LogData logData;
            logData.categories = _categories;
            logData.logs.swap(_logSeq);
            logDataSeq.push_back(logData);
	    bool ret = _client->doSend(logDataSeq);
            _strlen = 0;
	    return ! ret;
        }
      _dataMutex.notify();
        return 0;
    }

    template<typename P> inline string vector2String(vector<P> v){
        stringstream ss;
        ss << "["; 
        for(int i = 0; i < v.size(); i++){
            ss << boost::lexical_cast<string>(v[i]);
            if(i < v.size() -1){
                ss << ","; 
            }
        }
        ss << "]";
        return ss.str();
    }

}
