/*
 * EdgeListLinkImpl.cpp
 *
 *  Created on: 2012-3-28
 *      Author: benjamin
 */

#include "EdgeListLinkImpl.h"
using namespace com::renren::search;

EdgeListLinkImpl::EdgeListLinkImpl()
{
	this->head_ = NULL;
}

State *EdgeListLinkImpl::get(BYTE b)
{
	Cons *cons = this->head_;
	while (cons != NULL)
	{
		if (cons->b == b)
		{
			return cons->state;
		}
		else
		{
			cons = cons->next;
		}
	}
	return NULL;
}

void EdgeListLinkImpl::put(BYTE b, State *state)
{
	//struct Cons cons = {b, this->head_, state};
	//this->head_ = &cons;
	struct Cons *cons = new Cons;
	cons->b = b;
	cons->next = this->head_;
	cons->state = state;
	this->head_ = cons;
}

BYTE *EdgeListLinkImpl::keys()
{
	Cons *cons = this->head_;
	int key_size = 0;
	while (cons != NULL) {
		key_size++;
		cons = cons->next;
	}
	cons = this->head_;
	BYTE *keys = new BYTE[key_size + 1];
	int pos = 0;
	while (cons != NULL) {
		keys[pos] = cons->b;
		cons = cons->next;
		pos++;
	}
	keys[key_size] = '\0';
	return keys;
}
void EdgeListLinkImpl::deleteLink(struct Cons *cons)
{
	if (cons->next != NULL) {
		this->deleteLink(cons->next);
	} 
	delete cons->state;
	delete cons;
	return;
}

EdgeListLinkImpl::~EdgeListLinkImpl()
{
	if (this->head_ != NULL) {
		this->deleteLink(this->head_);
	}
}
