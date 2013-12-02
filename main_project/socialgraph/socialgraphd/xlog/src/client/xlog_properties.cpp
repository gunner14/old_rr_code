#include "src/client/xlog_properties.h"
#include "boost/algorithm/string.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>

using namespace std;

namespace xlog{
    vector<string>* getAgentHosts(const string& hostsStr);

    struct LocaleBool {
        bool data;
        LocaleBool() {}
        LocaleBool( bool data ) : data(data) {}
        operator bool() const { return data; }
        friend std::ostream & operator << ( std::ostream &out, LocaleBool b ) {
            out << std::boolalpha << b.data;
            return out;
        }
        friend std::istream & operator >> ( std::istream &in, LocaleBool &b ) {
            in >> std::boolalpha >> b.data;
            return in;
        }
    };

    string XLogProperties::DEFAULT_HOSTS = "xlogagent1.d.xiaonei.com:10001,xlogagent2.d.xiaonei.com:10001,xlogagent3.d.xiaonei.com:10001";
    const int XLogProperties::MAX_SEND_SIZE;
    const int XLogProperties::MAX_QUEUE_SIZE;


    XLogProperties::XLogProperties(const char* path){
        string hostsStr = DEFAULT_HOSTS ;
        int is_udp_protocol = true;
        int maxSendSize= 0;
        int maxQueueSize= 100000;

        ifstream conf_file(path);
        if(conf_file.is_open()){
            string line;
            vector<string> keyValue;
            string key;
            string value;
            while(conf_file.good()){
                getline(conf_file, line);
                int found = (int) line.find('#');
                if(found >= 0 ){
                    line = line.substr(0, found);
                }
                if(line.empty()){
                    continue;
                }
                boost::algorithm::trim(line);
                keyValue = boost::algorithm::split(keyValue, line, boost::algorithm::is_any_of("="));
                if(keyValue.size() < 2) {
                    throw invalid_argument("some lines has no value in the properties file " + *path);
                }
                key = keyValue[0];
                value= keyValue[1];
                boost::algorithm::trim(key);
                boost::algorithm::trim(value);
                if("hosts" == key){
                    hostsStr = value;
                }else if("isUdpProtocol" == key){
                    is_udp_protocol =  boost::lexical_cast<xlog::LocaleBool>(value);
                }else if("maxSendSize" == key){
                    maxSendSize = boost::lexical_cast<int>(value);
                }else if("maxQueueSize" == key){
                    maxQueueSize= boost::lexical_cast<int>(value);
                }else if("debug" == key){
                    //TODO
                }
            }
        } else{
            //TODO
        }

        _is_udp_protocol = is_udp_protocol;
        _maxSendSize = maxSendSize;
        _maxQueueSize = maxQueueSize;
        _hosts = xlog::getAgentHosts(hostsStr);
    }

    XLogProperties::XLogProperties(const string& hostsStr, const bool is_udp_protocol, const int maxSendSize, int maxQueueSize):_is_udp_protocol(is_udp_protocol), _maxSendSize(maxSendSize), _maxQueueSize(maxQueueSize){
        _hosts = xlog::getAgentHosts(hostsStr);
    }

    int XLogProperties::getMaxQueueSize() const {
        return (_maxQueueSize > 0 && _maxQueueSize < MAX_QUEUE_SIZE) ? _maxQueueSize : MAX_QUEUE_SIZE;
        //return (_maxQueueSize > MAX_QUEUE_SIZE) ? MAX_QUEUE_SIZE : (_maxQueueSize < 0) ? MAX_QUEUE_SIZE : _maxQueueSize;
    }

    int XLogProperties::getMaxSendSize() const {
        return (_maxSendSize > MAX_SEND_SIZE) ? MAX_SEND_SIZE : (_maxSendSize < 0) ? 0 : _maxSendSize;
    }
    vector<string> XLogProperties::getHosts() const{
        return *_hosts;
    }

    bool XLogProperties::isUdpProtocol() const{
        return _is_udp_protocol;
    }

    vector<string>* getAgentHosts(const string& hostsStr){
        vector<string>* agent_seq = new vector<string>();
        string tempStr(hostsStr);
        boost::algorithm::trim(tempStr);
        boost::algorithm::split(*agent_seq , tempStr, boost::algorithm::is_any_of(","));
        return agent_seq;
    }

}
