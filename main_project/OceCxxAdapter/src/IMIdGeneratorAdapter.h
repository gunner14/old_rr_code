#ifndef IMIDGENERATOR_ADAPTER_H__
#define IMIDGENERATOR_ADAPTER_H__

#include "Singleton.h"
#include "AdapterI.h"
#include "IMIdGenerator.h"
namespace xce {
namespace xntalk {

//class IMIdGeneratorAdapter: public MyUtil::AdapterI, public MyUtil::AdapterISingleton<MyUtil::TalkChannel, IMIdGeneratorAdapter> {
class IMIdGeneratorAdapter: public MyUtil::ReplicatedClusterAdapterI<IMIdGeneratorManagerPrx>, public MyUtil::Singleton<IMIdGeneratorAdapter> {
public:
  /*
  IMIdGeneratorAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
  */
  IMIdGeneratorAdapter() :MyUtil::ReplicatedClusterAdapterI<IMIdGeneratorManagerPrx> ("ControllerIMIdGenerator",120,300,new MyUtil::XceFeedControllerChannel) {}
  Ice::Long getId(Ice::Int type);
  Ice::Long createType(Ice::Int type, Ice::Long id, std::string note);
  Ice::Long createTypeWithNote(Ice::Int type, std::string note);

	IMIdGeneratorManagerPrx getManager(int id);
	IMIdGeneratorManagerPrx getManagerOneway(int id);
private:
 /*
  virtual std::string name() {
    return "M";
  }
  virtual std::string endpoints() {
    return "@IMIdGenerator";
  }
  virtual size_t cluster() {
    return 1;
  }
  std::vector<IMIdGeneratorManagerPrx> _managersOneway;
  std::vector<IMIdGeneratorManagerPrx> _managers;
  */
};

};
};


#endif
