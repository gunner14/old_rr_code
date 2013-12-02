/*
 * EdgeListArryImplArryImpl.cpp
 *
 *  Created on: 2012-3-20
 *      Author: benjamin
 */

#include "EdgeListArryImpl.h"
using namespace com::renren::search;
EdgeListArryImpl::EdgeListArryImpl()
{
	this->states_ = new State*[MAX_BYTE];
	for (int i = 0; i < MAX_BYTE; i++)
	{
		this->states_[i] = NULL;
	}
}

State* EdgeListArryImpl::get(BYTE b)
{
	return this->states_[(int) b & 0xFF];
}

void EdgeListArryImpl::put(BYTE b, State *state)
{
	this->states_[(int) b & 0xFF] = state;
}
BYTE* EdgeListArryImpl::keys()
{
	int key_size = 0;
	//	State *states = *(this->states_);
	for (int i = 0; i < MAX_BYTE; i++)
	{
		if (this->states_[i] != NULL)
		{
			key_size++;
		}
	}
	BYTE *keys = new BYTE[key_size + 1];
	for (int i = 0; i < key_size + 1; i++) {
		keys[i] = '\0';
	}
	for (int i = 0, pos = 0; i < MAX_BYTE; i++)
	{
		if (this->states_[i] != NULL)
		{
			keys[pos++] = (BYTE) i;
		}
	}
//	std::cout << strlen(keys) << std::endl;
	return keys;
}

EdgeListArryImpl::~EdgeListArryImpl()
{
	for (int i = MAX_BYTE; i > 0; i--)
	{
		if (this->states_[i] == NULL) {
			continue;
		} else {
			delete this->states_[i];
		}
	}
	delete[] this->states_;
}
