/*
 * EdgeList.h
 *
 *  Created on: 2012-3-25
 *      Author: benjamin
 */

#ifndef EDGELIST_H_
#define EDGELIST_H_
#include "AcTypes.h"

namespace com {
namespace renren {
namespace search {
class State;
class EdgeList
{
public:
	virtual State* get(BYTE b) = 0;
	virtual void put(BYTE b, State *state) = 0;
	virtual BYTE* keys() = 0;
	virtual ~EdgeList() {}
};
};
};
};
#endif /* EDGELIST_H_ */
