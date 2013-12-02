#ifndef __TALK_PROXY_ICE__
#define __TALK_PROXY_ICE__

#include <TalkCommon.ice>

module com
{
module xiaonei
{
module talk
{
  class HeartBeatConf {
    int oneBeatIntv;
    int floorBeatMoreGet;
    int maxIntvLoop;
    int minIntvLoop;
    int maxHeartUsers;
    int ceilOneCheckGet;

  };
  sequence<long> DeliverStatList;
	interface DeliverInterface{
		void express(common::Jid jid, string msg);
		void deliver(common::MessageSeq msgs);
	};
	
	interface ProxyManager extends DeliverInterface
	{
		bool indexExist(long index);
		bool sessionExist(long index);
		bool checkTicket(common::Jid jid , string ticket);
		bool verify(common::Jid jid, string ticket);
		string GetTicket(common::Jid jid);
		
		void leave(common::Jid jid);
		void relogin();
		
		void broadcast(string xmlMsg);
		//void event(common::EventSeq seq);
		void kick(common::Jid jid);
    DeliverStatList GetDeliverStat();
    void ClearDeliverStat();

    HeartBeatConf getHeartBeatConf();
    void setHeartBeatConf(HeartBeatConf conf);
    void opeHeartBeatThread(int flag);

	};
};
};
};

#endif
