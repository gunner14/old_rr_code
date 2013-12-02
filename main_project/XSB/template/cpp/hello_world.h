/**
 * @file   hello_world.h
 * @author liuyuan <yuan.liu1@renren-inc.com>
 * @date   Mon Aug 20 11:27:40 2012
 * 
 * @brief  This is a demo of how to make a c++ project
 *
 *         Copyright 2012 <renren-inc>
 */

#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include <string>

class HelloWorld {
public:
    ///< Constructor
    HelloWorld(const std::string &);

    ///< Virtual destructor
    virtual ~HelloWorld();

    /** 
     * Set the inner string, replace the old one
     * 
     */
    void SetInnerString(const std::string &);

    /** 
     * Get the inner string 
     * 
     * @return std::string type
     */
    std::string SayYaa();

public:
    ///< inner string hold the string
    std::string inner_string_;
};

#endif /* _HELLO_WORLD_H_ */

