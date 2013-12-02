#include "IMIdGeneratorAdapter.h"

using namespace MyUtil;
using namespace xce::xntalk;
IMIdGeneratorManagerPrx IMIdGeneratorAdapter::getManager(int id) {
//  return locate<IMIdGeneratorManagerPrx> (_managers, "M", id, TWO_WAY);
  return getProxy(id);
}

IMIdGeneratorManagerPrx IMIdGeneratorAdapter::getManagerOneway(int id) {
//  return locate<IMIdGeneratorManagerPrx> (_managersOneway, "M", id, ONE_WAY);
  return getProxyOneway(id);
}

Ice::Long IMIdGeneratorAdapter::getId(Ice::Int type) {
  return getManager(type)->getId(type);
}

Ice::Long IMIdGeneratorAdapter::createType(Ice::Int type, Ice::Long id, std::string note) {
  return getManager(type)->createType(type, id, note);
}
Ice::Long IMIdGeneratorAdapter::createTypeWithNote(Ice::Int type, std::string note) {
  return getManager(type)->createType(type, 1, note);
}
