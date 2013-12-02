#include "tripod2/cc/core/tripod_client.h"

namespace xce{
namespace journey {

  using namespace xce::tripod;
  
  class JourneyLogicCountTripodAdapter : public MyUtil::Singleton<JourneyLogicCountTripodAdapter> {
    public:
      ~JourneyLogicCountTripodAdapter();
      bool Set(const std::string& key, const std::string& value, int expire_time);
      bool Get(const std::string& key, std::string* value);
      std::string Inc(const std::string& key, const std::string& step);
      bool Remove(const std::string& key);
    private:
      friend class MyUtil::Singleton<JourneyLogicCountTripodAdapter>;
      JourneyLogicCountTripodAdapter();
      boost::shared_ptr<TripodClient> count_client_; 
      static const std::string kNamespaceId;
      static const std::string kBusinessId;  
  };
  
  class JourneyLogicListTripodAdapter : public MyUtil::Singleton<JourneyLogicListTripodAdapter> {
    public:
      ~JourneyLogicListTripodAdapter();
      bool GetSortedSet(const std::string& key, StringListType* zset_value, int begin, int limit);
      bool AddSortedSet(const std::string& key, const StringMapType& set_with_scores, int expire_time);
      bool TrimSortedSet(const std::string& key, int begin, int end);
      bool Remove(const std::string& key);
    private:
      friend class MyUtil::Singleton<JourneyLogicListTripodAdapter>;
      JourneyLogicListTripodAdapter();
      boost::shared_ptr<TripodClient> list_client_;
      static const std::string kNamespaceId;
      static const std::string kBusinessId;
  };

}}
