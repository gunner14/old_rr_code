#include "BadooServiceAdapter.h"

using namespace com::renren::xoa2::sns::badoo;

class deb_class {
  static int idpair[8][2];
  map<int, int> r2b;
  map<int, int> b2r;
  map<int, BadooLikeUserInfo> infos;
  map<int, BadooUserInfo> baseinfos;
  map<int, set<int> > like;
  map<int, set<int> > belike;

public:
  int get_bid(int id) {
    return (r2b.find(id) != r2b.end()) ? r2b[id] : 0;
  }
  int get_rid(int id) {
    return (b2r.find(id) != b2r.end()) ? b2r[id] : 0;
  }

  deb_class() {
    MCE_DEBUG("a:deb_class--> " << sizeof(idpair)/sizeof(idpair[0]) << "fuck fuck fuck fuck fuckfuckfuckfuckfuckfuckfuckfuckfuckfuckfuckfuckfuck");
    for (int i = 0; i < (sizeof(idpair)/sizeof(idpair[0])); ++i) {
      MCE_DEBUG("a:deb_class--> " << i << "fuck fuck fuck fuck fuckfuckfuckfuckfuckfuckfuckfuckfuckfuckfuckfuckfuck");
      int rid = idpair[i][0];
      int bid = idpair[i][1];
      ostringstream os;
      r2b[rid] = bid;
      b2r[bid] = rid;
      BadooLikeUserInfo tmp;
      tmp.userId = rid;
      tmp.badooUserId = bid;
      os << "遇见test" << rid << bid;
      tmp.name = os.str();
      infos[bid] = tmp;
      MCE_DEBUG("a:deb_class--> bid" << bid << " name:" << infos[bid].name);

      BadooUserInfo btmp;
      btmp.userId = rid;
      btmp.badooUserId = bid;
      btmp.name = os.str();
      btmp.mood = "fucking";
      btmp.charm = 1000;
      btmp.tinyUrl = "www.fuck.com";
      btmp.sex = rid % 2;
      btmp.age = rid % 100;
      btmp.provinceName = "北京";
      btmp.cityName = "故宫";
      baseinfos[bid] = btmp;
      MCE_DEBUG("a:deb_class--> bid" << bid << " name:" << baseinfos[bid].name);



      like[bid] = set<int>();
      belike[bid] = set<int>();

    }
    like[get_bid(225521693)].insert(get_bid(225521694));
    belike[get_bid(225521694)].insert(get_bid(225521693));
    like[get_bid(225521693)].insert(get_bid(225521695));
    belike[get_bid(225521695)].insert(get_bid(225521693));

    like[get_bid(225521694)].insert(get_bid(225521693));
    belike[get_bid(225521693)].insert(get_bid(225521694));

    like[get_bid(448146523)].insert(get_bid(201737098));
    belike[get_bid(201737098)].insert(get_bid(448146523));

    belike[get_bid(448146523)].insert(get_bid(201737098));
    like[get_bid(201737098)].insert(get_bid(448146523));
    


  }

  void getLikeUser(BadooLikeInfoList& rv, const int32_t rId) {
    map<int, set<int> > &fd = like;
    map<int, set<int> >::iterator pos = fd.find(get_bid(rId));
    if (pos == fd.end()) {
      return;
    }
    for (set<int>::iterator it = pos->second.begin(); it != pos->second.end(); ++it) {
      if (infos.find(*it) != infos.end())
        rv.push_back(infos[*it]);
    }
  }

  void getBeLikeUser(BadooLikeInfoList& rv, const int32_t rId) {
    map<int, set<int> > &fd = belike;
    map<int, set<int> >::iterator pos = fd.find(get_bid(rId));
    if (pos == fd.end()) {
      return;
    }
    for (set<int>::iterator it = pos->second.begin(); it != pos->second.end(); ++it) {
      if (infos.find(*it) != infos.end())
        rv.push_back(infos[*it]);
    }

  }

  void getMutileLikeUser(BadooLikeInfoList& rv, const int32_t rId) {
    map<int, set<int> > &fd = belike;
    map<int, set<int> >::iterator pos = fd.find(get_bid(rId));
    if (pos == fd.end()) {
      return;
    }

    map<int, set<int> > &fd0 = like;
    map<int, set<int> >::iterator pos0 = fd0.find(get_bid(rId));
    if (pos0 == fd0.end()) {
      return;
    }


    for (set<int>::iterator it = pos->second.begin(); it != pos->second.end(); ++it) {
      if ((pos0->second.find(*it) != pos0->second.end()) && (infos.find(*it) != infos.end()))
        rv.push_back(infos[*it]);
    }

  }


  void getUserBaseInfoByRId(BadooUserInfoList& rv, const IdWrappers& rIds) {
    for (IdWrappers::const_iterator it = rIds.begin(); it != rIds.end(); ++it) {
      
      if (baseinfos.find(get_bid(it->id)) != baseinfos.end()) {
        rv.push_back(baseinfos[get_bid(it->id)]);
        MCE_DEBUG("a::getUserBaseInfoByRId-->"<< it->id << " " << get_bid(it->id) << " " << baseinfos[get_bid(it->id)].name);
      }
    }

    MCE_DEBUG("a::getUserBaseInfoByRId-->"<< rv.size() << " " << rIds.size());
  }

  void getUserBaseInfoByBId(BadooUserInfoList& rv, const IdWrappers& bIds) {
    for (IdWrappers::const_iterator it = bIds.begin(); it != bIds.end(); ++it) {
      if (baseinfos.find(it->id) != baseinfos.end())
        rv.push_back(baseinfos[it->id]);
    }

  }

};

int deb_class::idpair[8][2] = {
  {448146523, 123},
  {201737098, 456},
  {438754937, 789},
  {225521693, 3333},
  {225521694, 4444},
  {225521695, 5555},
  {12345, 760882634},
  {67890, 760882763},
};

//static deb_class g_debc;


int32_t BadooAdapter::getBadooIdByRId(const int32_t rId) {
  XOA2_RPC_RETURN_CALL(BadooServiceClient, getBadooIdByRId, 0, (rId));

  //return g_debc.get_bid(rId);
}

int32_t BadooAdapter::getRIdByBId(const int32_t bId) {
  XOA2_RPC_RETURN_CALL(BadooServiceClient, getRIdByBId, 0, (bId));

  //return g_debc.get_rid(bId);


}


int32_t BadooAdapter::sendGossip(const int32_t senderBId, const int32_t receiverBId, const std::string& message) {
  XOA2_RPC_RETURN_CALL(BadooServiceClient, sendGossip, 0, (senderBId, receiverBId, message));
  
}

void BadooAdapter::getLikeUser(BadooLikeInfoList& rv, const int32_t rId, const int32_t limit) {
  XOA2_RPC_CALL(BadooServiceClient, getLikeUser, 0, (rv, rId, limit));

  //g_debc.getLikeUser(rv, rId);

}

void BadooAdapter::getBeLikeUser(BadooLikeInfoList& rv, const int32_t rId, const int32_t limit) {
  XOA2_RPC_CALL(BadooServiceClient, getBeLikeUser, 0, (rv, rId, limit));

  //g_debc.getBeLikeUser(rv, rId);

}
 
void BadooAdapter::getMutileLikeUser(BadooLikeInfoList& rv, const int32_t rId, const int32_t limit) {
  XOA2_RPC_CALL(BadooServiceClient, getMutileLikeUser, 0, (rv, rId, limit));

  //g_debc.getMutileLikeUser(rv, rId);
}

void BadooAdapter::getUserBaseInfoByRId(BadooUserInfoList& rv, const IdWrappers& rIds) {
  XOA2_RPC_CALL(BadooServiceClient, getUserBaseInfoByRId, 0, (rv, rIds));

  //g_debc.getUserBaseInfoByRId(rv, rIds);
}

void BadooAdapter::getUserBaseInfoByBId(BadooUserInfoList& rv, const IdWrappers& bIds) {
  XOA2_RPC_CALL(BadooServiceClient, getUserBaseInfoByBId, 0, (rv, bIds));

  //g_debc.getUserBaseInfoByBId(rv, bIds);

}


