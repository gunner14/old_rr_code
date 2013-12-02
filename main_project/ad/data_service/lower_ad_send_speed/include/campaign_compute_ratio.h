#include "compute_ratio.h"

namespace xce{
namespace ad{

class CampaignComputeRatio : public ComputeRatio{
public:
  virtual ~CampaignComputeRatio() {}
  virtual double Compute(std::map<std::string, double>& data_map);
};//end of class MemberComputeRatio
}//end of namespace ad
}//end of namespace xce
