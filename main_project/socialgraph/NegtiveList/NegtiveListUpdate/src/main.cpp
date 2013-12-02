//modified by   zhang zizhong
//e-mail:       zizhong.zhang
//Date:         2012年 02月 29日 星期三 10:25:07 CST

#include "PreloadLoginData.h"
#include "ReadFriendListUtil.h"
#include "SortByFriendRankUtil.h"
#include "socialgraph/socialgraphutil/clogging.h" // for clog
using namespace std;
using namespace xce::logindata;
using namespace com::xiaonei::xce;
using namespace readfriend;
using namespace sortbyrank;
using namespace xce::socialgraph;

SortByFriendRankUtil* g_sortbyrank_ptr = NULL;
ReadFriendListUtil* g_readfriend_ptr = NULL;

void Init();
void End();
void PrintTime();
char* time2str(const time_t ltime, char* str);

int main(int argc, char* argv[]) {
	
    INIT_CLOG("negtivelist");
    Init();
	PrintTime();

    PreloadLoginData worker;
    worker.loadFromDB();      //加载数据

	End();
}

void Init() {	
    ConnectionPoolManager::instance().initialize();
	g_sortbyrank_ptr = new SortByFriendRankUtil();
    g_readfriend_ptr = new ReadFriendListUtil();
}

void PrintTime() {
    time_t now = time(NULL);
    char date_str[20];
    time2str(now, date_str);  
    CLOG(INFO) << "+---------------------------+";
    CLOG(INFO) << "| " << date_str << "       |";
    CLOG(INFO) << "+---------------------------+";
}

char* time2str(const time_t ltime, char* str) {                                                                                                                                  
    if (str == NULL) {
        return NULL;
    }

    struct tm *Tm = localtime(&ltime);
    sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d",
            Tm->tm_year + 1900,
            Tm->tm_mon + 1,           
            Tm->tm_mday,
            Tm->tm_hour,
            Tm->tm_min,
            Tm->tm_sec);
    return str;
}

void End() {
    delete g_sortbyrank_ptr;
    delete g_readfriend_ptr;
}
