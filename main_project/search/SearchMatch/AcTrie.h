/*
 * AcTrie.h
 *
 *  Created on: 2012-3-21
 *      Author: benjamin
 */

#ifndef ACTRIE_H_
#define ACTRIE_H_
#include <queue>
#include "State.h"
namespace com
{
namespace renren
{
namespace search
{
class AcTrie
{
private:
	State *root;
	bool prepared;
	void prepareFailTransitions();
	void prepareRoot();
public:
	AcTrie();
	void addNode(BYTE *bytes, std::string output, int id, int st);
	void prepare();
	State *getRoot();
	bool isPrepared();
	virtual ~AcTrie();
};
//typedef IceUtil::Handle<AcTrie> AcTriePtr;
};
};
};

#endif /* ACTRIE_H_ */
