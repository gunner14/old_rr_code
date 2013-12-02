/*
 * SearchResult.h
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#ifndef SEARCHRESULT_H_
#define SEARCHRESULT_H_
#include "AcTypes.h"
#include "State.h"
namespace com
{
namespace renren
{
namespace search
{
class SearchResult
{
private:
	State *lastMatchedState_;
	BYTE *bytes_;
	int lastIndex_;
	int minusCount_;
public:
	SearchResult(State *state, BYTE *bytes, int lastIndex, int minusCount);
	State *getLastMatchedState();
	BYTE *getBytes();
	int getLastIndex();
	int getMinusCount();
	virtual ~SearchResult();
};
};
};
};

#endif /* SEARCHRESULT_H_ */
