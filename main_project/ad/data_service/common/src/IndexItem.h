/*
 * =====================================================================================
 *
 *       Filename:  IndexItem.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/09/12 16:48:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __INDEX_ITEM_H__
#define __INDEX_ITEM_H__
#include <string>
#include <vector>
#include <iostream>

namespace xce{
namespace ad{
struct IndexItem{
	std::string table; 
	std::vector<std::string> fields;

	IndexItem(const std::string &t):table(t){}

	size_t size() const{
		return fields.size();
	}
};
}
}

#endif
