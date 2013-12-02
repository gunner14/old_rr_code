/*
 * Searcher.cpp
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#include "Searcher.h"
using namespace com::renren::search;
Searcher::Searcher(AcTrie &acTrie, BYTE *bytes) : acTrie_(acTrie)
{
	//this->acTrie_ = acTrie;
	if (!this->acTrie_.isPrepared())
	{
		throw "Can not start search until prepare() called.";
	}
	else
	{
		SearchResult *result = new SearchResult(this->acTrie_.getRoot(), bytes, 0, 0);
		this->searchResult_ = this->search(result);
		delete result;
	}
}

SearchResult *Searcher::search(SearchResult *searchResult)
{
	BYTE *bytes = searchResult->getBytes();
	State *state = searchResult->getLastMatchedState();
	int lastIndex = searchResult->getLastIndex();
	int minusCount = searchResult->getMinusCount();

	//delete searchResult;
	//searchResult = NULL;
	int length = strlen(bytes);
//	GET_ARRAY_LEN(bytes, length);

	for (int i = lastIndex; i < length; i++)
	{
		BYTE b = bytes[i];

		if (b >= -127 && b <= -64)
		{
			minusCount++;
		}

		while (state->getEdgeList()->get(b) == NULL)
		{
			state = state->getFail();
		}

		state = state->getEdgeList()->get(b);
		if (state->getOutput()->size() > 0)
		{
			return new SearchResult(state, bytes, i + 1, minusCount);
		}
	}
	return NULL;
}

bool Searcher::hasNext() {
	return this->searchResult_ != NULL;
}

SearchResult *Searcher::next() {
	if (!this->hasNext()) {
		throw "No element";
	}
	SearchResult *result = this->searchResult_;
	this->searchResult_ = this->search(result);

	return result;
}

Searcher::~Searcher()
{
	if (searchResult_ != NULL) {
		delete searchResult_;
	}
}
