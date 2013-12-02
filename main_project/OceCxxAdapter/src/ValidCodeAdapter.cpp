#include "ValidCodeAdapter.h"

using namespace com::xiaonei::icode::xoa;

bool ValidCodeAdapter::validCode(const std::string &inputCode, const std::string &type, const std::string &id) {
  XOA2_RPC_RETURN_CALL(ValidCodeServiceClient, validCode, 0, (inputCode, type, id));
}
