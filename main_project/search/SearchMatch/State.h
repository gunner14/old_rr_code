/*
 * State.h
 *
 *  Created on: 2012-3-20
 *      Author: benjamin
 */

#ifndef STATE_H_
#define STATE_H_
#include <iostream>
#include "AcTypes.h"
#include "EdgeListArryImpl.h"
#include "EdgeListLinkImpl.h"
namespace com
{
namespace renren
{
namespace search
{
class EdgeList;
class EdgeListArryImpl;
class EdgeListLinkImpl;
class State
{
private:
	short depth_;
	OutputMap *output_;
	State* fail_;
	EdgeList* edgelistI_;
public:
	State(short depth);
	virtual ~State();
	State *addEdge(BYTE);
	State *addEdgeAll(BYTE*);

	EdgeList *getEdgeList();
	void setEdgeList(EdgeList *edgelist);
	State *getFail();
	void setFail(State *fail);
	OutputMap *getOutput();
	void addOutput(int i, std::string output, int st);
	void setOutput(OutputMap *output);
};
}
;
}
;
}
;
#endif /* STATE_H_ */
