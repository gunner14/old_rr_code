#ifndef __USERNETWORK_ADAPTER_H__
#define __USERNETWORK_ADAPTER_H__

#include "UserNetworkReaderAdapter.h"
#include "UserNetworkWriterAdapter.h"

namespace mop {
namespace hi {
namespace svc {
namespace adapter {
using namespace MyUtil;
using namespace mop::hi::svc::model;

class UserNetworkAdapter : public Singleton<UserNetworkAdapter> {
public:
  //以下get方法兼容NetworkManager提供的接口
  NetworkBigObjectNPtr getBigObjectN(int userId);
  HighSchoolInfoSeq getHighSchoolInfoSeq(int userId);
  NetworkManagerPrx getNetworkManager(int userId);
  NetworkManagerPrx getNetworkManagerOneway(int userId);

  //以下get方法为checker提供
  NetworkDataPtr getNetworkFullData(int userId);

  //以下get方法为人人桌面提供
  NetworkSeq getNetworkSeq(int userId);
};

}}}} //namespace mop::hi::svc::adapter

#endif
