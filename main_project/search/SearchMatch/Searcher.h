/*
 * Searcher.h
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#ifndef SEARCHER_H_
#define SEARCHER_H_
#include "SearchResult.h"
#include "AcTrie.h"
namespace com
{
namespace renren
{
namespace search
{
class Searcher
{
private:
	SearchResult *searchResult_;
	AcTrie &acTrie_;
public:
	Searcher(AcTrie &acTrie, BYTE *bytes);
	SearchResult *search(SearchResult *searchResult);
	bool hasNext();
	SearchResult *next();
	virtual ~Searcher();
};
};
};
};

#endif /* SEARCHER_H_ */
