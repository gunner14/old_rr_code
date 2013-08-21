#ifndef _RR_ACTION_RUNIMAGE_H_
#define _RR_ACTION_RUNIMAGE_H_
#include <vector>
#include <map>
#include "cwf/action.h"
#include <boost/scoped_ptr.hpp>
namespace rtimage {
class GrowBuffer;
class GrowBufs;
class JpgHandler;
class PngHandler;
class GifHandler;
class WatermarkParam;
}
;

namespace runimage {

using namespace cwf;
class RunImageAction: public BaseAction {
public:
  RunImageAction();
  ~RunImageAction();
  void Stop();
private:
  bool Match(const std::string& url) const;
  HttpStatusCode Process(Request*, Response*);
private:
  rtimage::GrowBufs* growbufs_;
  boost::scoped_ptr<int> test_;
  boost::scoped_ptr<rtimage::GrowBuffer> filedata_;
  std::string filestring_;
  boost::scoped_ptr<rtimage::JpgHandler> jpg_;
  boost::scoped_ptr<rtimage::PngHandler> png_;
  boost::scoped_ptr<rtimage::GifHandler> gif_;
  boost::scoped_ptr<rtimage::WatermarkParam> watermark_param_;
};

}
#endif
