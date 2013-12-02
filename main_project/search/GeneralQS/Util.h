/*
 * Util.h
 *
 *  Created on: 2012-8-24
 *      Author: cm
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Util {
public:
	Util();

	static string& trim(string& str);
	static vector<string> tokenize(string& text, string seq);
	static void union_vec(std::vector<int>&A, const std::vector<int>& B);
	static bool startsWith(const std::string& base, const std::string& pre);
	virtual ~Util();
};

#endif /* UTIL_H_ */
