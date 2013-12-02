// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `ImplicitContext.ice'

#ifndef __Ice_ImplicitContext_h__
#define __Ice_ImplicitContext_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/LocalException.h>
#include <Ice/Current.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

#ifndef ICE_API
#   ifdef ICE_API_EXPORTS
#       define ICE_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace Ice
{

class ImplicitContext;
ICE_API bool operator==(const ImplicitContext&, const ImplicitContext&);
ICE_API bool operator<(const ImplicitContext&, const ImplicitContext&);

}

namespace IceInternal
{

ICE_API ::Ice::LocalObject* upCast(::Ice::ImplicitContext*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::ImplicitContext> ImplicitContextPtr;

}

namespace Ice
{

class ICE_API ImplicitContext : virtual public ::Ice::LocalObject
{
public:

    typedef ImplicitContextPtr PointerType;
    

    virtual ::Ice::Context getContext() const = 0;

    virtual void setContext(const ::Ice::Context&) = 0;

    virtual bool containsKey(const ::std::string&) const = 0;

    virtual ::std::string get(const ::std::string&) const = 0;

    virtual ::std::string put(const ::std::string&, const ::std::string&) = 0;

    virtual ::std::string remove(const ::std::string&) = 0;
};

}

#endif