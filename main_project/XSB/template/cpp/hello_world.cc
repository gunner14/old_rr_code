/**
 * @file   hello_world.cc
 * @author liuyuan <yuan.liu1@renren-inc.com>
 * @date   Mon Aug 20 11:39:33 2012
 * 
 * @brief  This is a demo of how to make a c++ project
 *
 *         Copyright 2012 <renren-inc>
 */

#include "hello_world.h"

using std::string;

HelloWorld::HelloWorld(const string &str) {
    SetInnerString(str);
}

HelloWorld::~HelloWorld() {}

void
HelloWorld::SetInnerString(const string &str) {
    inner_string_ = str;
}

string
HelloWorld::SayYaa() {
    return inner_string_;
}
