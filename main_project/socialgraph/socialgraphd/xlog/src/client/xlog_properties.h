#ifndef __XLOG_PROPERTIES__
#define __XLOG_PROPERTIES__

#include <string>
#include <vector>
using namespace std;
namespace xlog{
    class XLogProperties{
        public:
            //const static int MAX_SEND_SIZE;
            //const static int MAX_QUEUE_SIZE;
            static const int MAX_SEND_SIZE = 50000;
            static const int MAX_QUEUE_SIZE = 100000;
            static string DEFAULT_HOSTS;
            XLogProperties(const char* path);
            XLogProperties(const string& hosts, const bool is_udp_protocol, const int maxSendSize, int maxQueueSize);
            bool isUdpProtocol() const;
            int getMaxSendSize() const;
            int getMaxQueueSize() const;
            vector<string> getHosts() const;
        private:
            vector<string>* _hosts;
            bool _is_udp_protocol;
            int _maxSendSize;
            int _maxQueueSize;

    };


}
#endif
