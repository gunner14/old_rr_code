#include "compute_ratio.h"

namespace xce{
namespace ad{

class MemberComputeRatio : public ComputeRatio{
public:
  virtual ~MemberComputeRatio() {}
  virtual double Compute(std::map<std::string, double>& dataMap);
};//end of class MemberComputeRatio
}//end of namespace ad
}//end of namespace xce
