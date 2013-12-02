#ifndef USER_SCORE_TRIPOD_CLIENT_H_
#define USER_SCORE_TRIPOD_CLIENT_H_

#include <string>
#include "boost/shared_ptr.hpp"
#include "Singleton.h"
#include "ScoreCache.h"
#include "tripod2/cc/core/single_table_tripod_client.h"
#include "tripod2/cc/core/multibiz_tripod_client.h"
#include "OceProto/proto/user_score.pb.h"
#include "score/ScoreCache/share/ScoreCacheData.h"

namespace xce {
namespace adapter {
using namespace xce::tripod;
using namespace xce::scorecache;
using namespace xce::tripoddata::userscore;

class UserScoreTripodAdapter : public MyUtil::Singleton<UserScoreTripodAdapter> {
  private:
    boost::shared_ptr<SingleTableTripodClient<PbScoreLogin, long> > score_login_client_;
    boost::shared_ptr<SingleTableTripodClient<PbUserScore, long> > score_client_;
    boost::shared_ptr<SingleTableTripodClient<PbScoreAwardCount, long> > score_award_count_client_;
    boost::shared_ptr<SingleTableTripodClient<PbScoreSign, long> > score_sign_client_;
    boost::shared_ptr<SingleTableTripodClient<PbScoreVip, long> > score_vip_client_;
    boost::shared_ptr<MultiBizTripodClient> multi_biz_client_;
    std::list<std::string> biz_all_of_score;
    static const std::string kUserScoreNamespace;
    static const std::string kBizScoreLogin;
    static const std::string kBizScore;
    static const std::string kBizScoreAwardCount;
    static const std::string kBizScoreSign;
    static const std::string kBizScoreVip;
   
  public:
    UserScoreTripodAdapter(); 
    ScoreDataN GetScoreDataN(long user_id);
    ScoreDataN GetScoreDataN2(long user_id);
    ScoreDataNMap GetScoreDataNMap(const std::list<long>& id);
    ScoreDataNMap GetScoreDataNMap2(const std::list<long>& id);
    bool LoadScoreLogin(long user_id);
    bool LoadScore(long user_id);
    bool LoadScoreAwardCount(long user_id);
    bool LoadScoreSign(long user_id);
    bool LoadScoreVip(long user_id);
    bool LoadAll(long user_id);
    virtual ~UserScoreTripodAdapter();
    void PrintScoreDataN(const ScoreDataN& data);
};

class UserScoreUtil {
  public:
  static void parse(const PbScoreLogin& score_login, ScoreCacheDataNIPtr &cache_data);
  static void parse(const PbUserScore& score, ScoreCacheDataNIPtr& cache_data);
  static void parse(const PbScoreSign& score_sign, ScoreCacheDataNIPtr& cache_data);
  static void parse(const PbScoreAwardCount& score_award, ScoreCacheDataNIPtr& cache_data);
  static void parse(const PbScoreVip& score_vip, ScoreCacheDataNIPtr& cache_data);
};

}}// namespace

#endif
