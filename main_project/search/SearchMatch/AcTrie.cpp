/*
 * AcTrie.cpp
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#include "AcTrie.h"
using namespace com::renren::search;
AcTrie::AcTrie()
{
	root = new State(0);
	prepared = false;
}

void AcTrie::addNode(BYTE *bytes, std::string output, int id, int st)
{
	if (this->prepared)
	{
		throw "Can not add node after prepare() is called.";
	}
	State *lastState = this->root->addEdgeAll(bytes);
	//	std::string *outputs = lastState->getOutput();
	// TODO add the output to the array
	//	outputs = new std::string[1];
	//	outputs[0] = output;
	lastState->addOutput(id, output, st);
}

void AcTrie::prepare()
{
	this->prepareFailTransitions();
	this->prepared = true;
}
void AcTrie::prepareFailTransitions()
{
	std::queue<State*> queue;
	for (int i = 0; i < MAX_BYTE; i++)
	{
		State *state = this->root->getEdgeList()->get((BYTE) i);
		if (state != NULL)
		{
			state->setFail(this->root);
			queue.push(state);
		}
	}
	this->prepareRoot();

	while (!queue.empty())
	{
		State *state = queue.front();
		queue.pop();
		BYTE *keys = state->getEdgeList()->keys();
		int length = strlen(keys);
		for (int i = 0; i < length; i++)
		{
			BYTE b = keys[i];
			State *rt = state;
			State *nd = state->getEdgeList()->get(b);
			queue.push(nd);

			rt = rt->getFail();
			while (rt->getEdgeList()->get(b) == NULL)
			{
				rt = rt->getFail();
			}

			nd->setFail(rt->getEdgeList()->get(b));
			for (OutputMap::iterator aIterator = rt->getEdgeList()->get(
					b)->getOutput()->begin(); aIterator
					!= rt->getEdgeList()->get(b)->getOutput()->end(); ++aIterator)
			{
				//Send it to deletion
				if (aIterator->second.name != "") {
					if (nd->getOutput()->find(aIterator->first) == nd->getOutput()->end()) {
						nd->addOutput(aIterator->first, aIterator->second.name, aIterator->second.st);
						std::cout << "id: " << aIterator->first << " str: " << aIterator->second.name << std::endl;;
					}
				}
			}
		}
		delete[] keys;
	}
}

void AcTrie::prepareRoot()
{
	for (int i = 0; i < MAX_BYTE; i++)
	{
		if (this->root->getEdgeList()->get((BYTE) i) == NULL)
		{
			this->root->getEdgeList()->put((BYTE) i, this->root);
		}
	}
}
State * AcTrie::getRoot()
{
	return this->root;
}

bool AcTrie::isPrepared()
{
	return this->prepared;
}

AcTrie::~AcTrie()
{
	delete root;
}
