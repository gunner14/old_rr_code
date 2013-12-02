/*
 * State.cpp
 *
 *  Created on: 2012-3-20
 *      Author: benjamin
 */

#include "State.h"
using namespace com::renren::search;
State::State(short depth)
{
	this->depth_ = depth;
	this->fail_ = NULL;
	if (depth >= THRESHOLD_TO_USE_SPARSE) {
		this->edgelistI_ = new EdgeListLinkImpl;
	} else {
		this->edgelistI_ = new EdgeListArryImpl;
	}
	this->output_ = new OutputMap;
}

State *State::addEdge(BYTE b)
{
	State *s = this->edgelistI_->get(b);
	if (s != NULL)
	{
		return s;
	}
	else
	{
		State *newState = new State(this->depth_ + 1);
		this->edgelistI_->put(b, newState);
		return newState;
	}
}

State *State::addEdgeAll(BYTE *bytes)
{
	State *state = this;
	int length = strlen(bytes);
	for (int i = 0; i < length; i++)
	{
		State *s = state->getEdgeList()->get(bytes[i]);
		if (s != NULL)
		{
			state = s;
		}
		else
		{
			state = state->addEdge(bytes[i]);
		}
	}
	return state;
}

EdgeList *State::getEdgeList()
{
	return this->edgelistI_;
}

State *State::getFail()
{
	return this->fail_;
}

void State::setFail(State *fail) {
	this->fail_ = fail;
}

OutputMap *State::getOutput()
{
	return this->output_;
}

void State::addOutput(int id, std::string output, int st) {
	NameState ns;
	ns.name = output;
	ns.st = st;
	this->output_->insert(std::pair<int, NameState> (id, ns));
}

void State::setOutput(OutputMap *output) {
	this->output_ = output;
}

State::~State()
{
	delete this->edgelistI_;
	delete this->fail_;
	delete this->output_;
}
