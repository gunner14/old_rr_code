/*
 * EdgeListArryImpl.h
 *
 *  Created on: 2012-3-20
 *      Author: benjamin
 */

#ifndef EDGELISTARRYIMPL_H_
#define EDGELISTARRYIMPL_H_
#include "EdgeList.h"

namespace com {
namespace renren {
namespace search {
class State;

class EdgeListArryImpl : public EdgeList
{
private:
	State **states_;
public:
	EdgeListArryImpl();
	virtual State* get(BYTE b);
	virtual void put(BYTE b, State *state);
	virtual BYTE* keys();
	virtual ~EdgeListArryImpl();
};

};
};
};

#endif /* EDGELISTARRYIMPL_H_ */
