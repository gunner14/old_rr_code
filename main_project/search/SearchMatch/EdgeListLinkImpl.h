/*
 * EdgeListLinkImpl.h
 *
 *  Created on: 2012-3-28
 *      Author: benjamin
 */

#ifndef EDGELISTLINKIMPL_H_
#define EDGELISTLINKIMPL_H_
#include "EdgeList.h"

namespace com
{
namespace renren
{
namespace search
{
class State;

struct Cons {
	BYTE b;
	struct Cons *next;
	State *state;
};

class EdgeListLinkImpl : public EdgeList {
private:
	Cons *head_;
	virtual void deleteLink(struct Cons *cons);
public:
	EdgeListLinkImpl();
	virtual State* get(BYTE b);
	virtual void put(BYTE b, State *state);
	virtual BYTE* keys();
	virtual ~EdgeListLinkImpl();
};
};
};
};

#endif /* EDGELISTLINKIMPL_H_ */
