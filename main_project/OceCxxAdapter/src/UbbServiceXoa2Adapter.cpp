#include "UbbServiceXoa2Adapter.h"

using namespace com::renren::xoa2::ugc::status;

void UbbServiceAdapter::getDoingUbbsBytypeNE(Xoa2UbbListResult& result,int32_t type) {
 
  XOA2_RPC_CALL( UbbServiceClient, getDoingUbbsBytypeNE, 0, (result,type));
}

