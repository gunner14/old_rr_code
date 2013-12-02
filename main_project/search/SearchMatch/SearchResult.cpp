/*
 * SearchResult.cpp
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#include "SearchResult.h"
using namespace com::renren::search;
SearchResult::SearchResult(State *state, BYTE *bytes, int lastIndex,
		int minusCount)
{
	this->lastMatchedState_ = state;
	this->bytes_ = bytes;
	this->lastIndex_ = lastIndex;
	this->minusCount_ = minusCount;
}

State * SearchResult::getLastMatchedState()
{
	return this->lastMatchedState_;
}

BYTE * SearchResult::getBytes()
{
	return this->bytes_;
}

int SearchResult::getLastIndex()
{
	return this->lastIndex_;
}

int SearchResult::getMinusCount()
{
	return this->minusCount_;
}
SearchResult::~SearchResult()
{
	//都不是new出来的，所以就不用delete了
	//delete this->lastMatchedState_;
	//delete this->bytes_;
}
