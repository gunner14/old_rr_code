// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `xlog.ice'

#include <xlog.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

static const ::std::string __xlog__slice__Subscriber__notify_name = "notify";

static const ::std::string __xlog__slice__Agent__add_name = "add";

static const ::std::string __xlog__slice__Agent__addFailedLogData_name = "addFailedLogData";

static const ::std::string __xlog__slice__Agent__subscribeClient_name = "subscribeClient";

static const ::std::string __xlog__slice__Agent__subscribeSubscriber_name = "subscribeSubscriber";

static const ::std::string __xlog__slice__Logger__add_name = "add";

static const ::std::string __xlog__slice__Logger__addLogData_name = "addLogData";

static const ::std::string __xlog__slice__Dispatcher__add_name = "add";

static const ::std::string __xlog__slice__Dispatcher__addLogData_name = "addLogData";

static const ::std::string __xlog__slice__Dispatcher__createZNode_name = "createZNode";

static const ::std::string __xlog__slice__Dispatcher__register_name = "register";

static const ::std::string __xlog__slice__Dispatcher__subscribe_name = "subscribe";

static const ::std::string __xlog__slice__Dispatcher__unsubscribe_name = "unsubscribe";

static const ::std::string __xlog__slice__Dispatcher__getData_name = "getData";

static const ::std::string __xlog__slice__PublisherService__publish_name = "publish";

::Ice::Object* IceInternal::upCast(::xlog::slice::Subscription* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::Subscription* p) { return p; }

::Ice::Object* IceInternal::upCast(::xlog::slice::PushSubscription* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::PushSubscription* p) { return p; }

::Ice::Object* IceInternal::upCast(::xlog::slice::Subscriber* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::Subscriber* p) { return p; }

::Ice::Object* IceInternal::upCast(::xlog::slice::Agent* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::Agent* p) { return p; }

::Ice::Object* IceInternal::upCast(::xlog::slice::Logger* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::Logger* p) { return p; }

::Ice::Object* IceInternal::upCast(::xlog::slice::Dispatcher* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::Dispatcher* p) { return p; }

::Ice::Object* IceInternal::upCast(::xlog::slice::PublisherService* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::xlog::slice::PublisherService* p) { return p; }

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::SubscriptionPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::Subscription;
        v->__copyFrom(proxy);
    }
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::PushSubscriptionPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::PushSubscription;
        v->__copyFrom(proxy);
    }
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::SubscriberPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::Subscriber;
        v->__copyFrom(proxy);
    }
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::AgentPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::Agent;
        v->__copyFrom(proxy);
    }
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::LoggerPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::Logger;
        v->__copyFrom(proxy);
    }
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::DispatcherPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::Dispatcher;
        v->__copyFrom(proxy);
    }
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::PublisherServicePrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::xlog::slice::PublisherService;
        v->__copyFrom(proxy);
    }
}

bool
xlog::slice::LogData::operator==(const LogData& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(categories != __rhs.categories)
    {
        return false;
    }
    if(logs != __rhs.logs)
    {
        return false;
    }
    if(checkSum != __rhs.checkSum)
    {
        return false;
    }
    return true;
}

bool
xlog::slice::LogData::operator<(const LogData& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(categories < __rhs.categories)
    {
        return true;
    }
    else if(__rhs.categories < categories)
    {
        return false;
    }
    if(logs < __rhs.logs)
    {
        return true;
    }
    else if(__rhs.logs < logs)
    {
        return false;
    }
    if(checkSum < __rhs.checkSum)
    {
        return true;
    }
    else if(__rhs.checkSum < checkSum)
    {
        return false;
    }
    return false;
}

void
xlog::slice::LogData::__write(::IceInternal::BasicStream* __os) const
{
    if(categories.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&categories[0], &categories[0] + categories.size());
    }
    if(logs.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&logs[0], &logs[0] + logs.size());
    }
    __os->write(checkSum);
}

void
xlog::slice::LogData::__read(::IceInternal::BasicStream* __is)
{
    __is->read(categories);
    __is->read(logs);
    __is->read(checkSum);
}

void
xlog::slice::__writeLogDataSeq(::IceInternal::BasicStream* __os, const ::xlog::slice::LogData* begin, const ::xlog::slice::LogData* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
xlog::slice::__readLogDataSeq(::IceInternal::BasicStream* __is, ::xlog::slice::LogDataSeq& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 3);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

void
xlog::slice::__writeStr2StrDict(::IceInternal::BasicStream* __os, const ::xlog::slice::Str2StrDict& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::xlog::slice::Str2StrDict::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        __os->write(p->second);
    }
}

void
xlog::slice::__readStr2StrDict(::IceInternal::BasicStream* __is, ::xlog::slice::Str2StrDict& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::std::string> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::xlog::slice::Str2StrDict::iterator __i = v.insert(v.end(), pair);
        __is->read(__i->second);
    }
}

void
xlog::slice::__write(::IceInternal::BasicStream* __os, ::xlog::slice::ErrorCode v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 3);
}

void
xlog::slice::__read(::IceInternal::BasicStream* __is, ::xlog::slice::ErrorCode& v)
{
    ::Ice::Byte val;
    __is->read(val, 3);
    v = static_cast< ::xlog::slice::ErrorCode>(val);
}

xlog::slice::XLogException::XLogException(::xlog::slice::ErrorCode __ice_code, const ::std::string& __ice_msg) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    code(__ice_code),
    msg(__ice_msg)
{
}

xlog::slice::XLogException::~XLogException() throw()
{
}

static const char* __xlog__slice__XLogException_name = "xlog::slice::XLogException";

::std::string
xlog::slice::XLogException::ice_name() const
{
    return __xlog__slice__XLogException_name;
}

::Ice::Exception*
xlog::slice::XLogException::ice_clone() const
{
    return new XLogException(*this);
}

void
xlog::slice::XLogException::ice_throw() const
{
    throw *this;
}

void
xlog::slice::XLogException::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::xlog::slice::XLogException"), false);
    __os->startWriteSlice();
    ::xlog::slice::__write(__os, code);
    __os->write(msg);
    __os->endWriteSlice();
}

void
xlog::slice::XLogException::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    ::xlog::slice::__read(__is, code);
    __is->read(msg);
    __is->endReadSlice();
}

void
xlog::slice::XLogException::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception xlog::slice::XLogException was not generated with stream support";
    throw ex;
}

void
xlog::slice::XLogException::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception xlog::slice::XLogException was not generated with stream support";
    throw ex;
}

struct __F__xlog__slice__XLogException : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::xlog::slice::XLogException();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__xlog__slice__XLogException__Ptr = new __F__xlog__slice__XLogException;

const ::IceInternal::UserExceptionFactoryPtr&
xlog::slice::XLogException::ice_factory()
{
    return __F__xlog__slice__XLogException__Ptr;
}

class __F__xlog__slice__XLogException__Init
{
public:

    __F__xlog__slice__XLogException__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::xlog::slice::XLogException", ::xlog::slice::XLogException::ice_factory());
    }

    ~__F__xlog__slice__XLogException__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::xlog::slice::XLogException");
    }
};

static __F__xlog__slice__XLogException__Init __F__xlog__slice__XLogException__i;

#ifdef __APPLE__
extern "C" { void __F__xlog__slice__XLogException__initializer() {} }
#endif

const ::std::string&
IceProxy::xlog::slice::Subscription::ice_staticId()
{
    return ::xlog::slice::Subscription::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::Subscription::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::Subscription);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::Subscription::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::Subscription);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::Subscription::__newInstance() const
{
    return new Subscription;
}

const ::std::string&
IceProxy::xlog::slice::PushSubscription::ice_staticId()
{
    return ::xlog::slice::PushSubscription::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::PushSubscription::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::PushSubscription);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::PushSubscription::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::PushSubscription);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::PushSubscription::__newInstance() const
{
    return new PushSubscription;
}

void
IceProxy::xlog::slice::Subscriber::notify(const ::Ice::StringSeq& config, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Subscriber* __del = dynamic_cast< ::IceDelegate::xlog::slice::Subscriber*>(__delBase.get());
            __del->notify(config, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::xlog::slice::Subscriber::ice_staticId()
{
    return ::xlog::slice::Subscriber::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::Subscriber::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::Subscriber);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::Subscriber::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::Subscriber);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::Subscriber::__newInstance() const
{
    return new Subscriber;
}

void
IceProxy::xlog::slice::Agent::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Agent* __del = dynamic_cast< ::IceDelegate::xlog::slice::Agent*>(__delBase.get());
            __del->add(data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::xlog::slice::Agent::addFailedLogData(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Agent* __del = dynamic_cast< ::IceDelegate::xlog::slice::Agent*>(__delBase.get());
            __del->addFailedLogData(data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::StringSeq
IceProxy::xlog::slice::Agent::subscribeClient(const ::std::string& prxStr, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__Agent__subscribeClient_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Agent* __del = dynamic_cast< ::IceDelegate::xlog::slice::Agent*>(__delBase.get());
            return __del->subscribeClient(prxStr, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::StringSeq
IceProxy::xlog::slice::Agent::subscribeSubscriber(const ::Ice::StringSeq& categories, const ::std::string& prxStr, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__Agent__subscribeSubscriber_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Agent* __del = dynamic_cast< ::IceDelegate::xlog::slice::Agent*>(__delBase.get());
            return __del->subscribeSubscriber(categories, prxStr, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::xlog::slice::Agent::ice_staticId()
{
    return ::xlog::slice::Agent::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::Agent::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::Agent);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::Agent::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::Agent);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::Agent::__newInstance() const
{
    return new Agent;
}

void
IceProxy::xlog::slice::Logger::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Logger* __del = dynamic_cast< ::IceDelegate::xlog::slice::Logger*>(__delBase.get());
            __del->add(data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::xlog::slice::Logger::addLogData(const ::xlog::slice::LogData& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Logger* __del = dynamic_cast< ::IceDelegate::xlog::slice::Logger*>(__delBase.get());
            __del->addLogData(data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::xlog::slice::Logger::ice_staticId()
{
    return ::xlog::slice::Logger::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::Logger::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::Logger);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::Logger::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::Logger);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::Logger::__newInstance() const
{
    return new Logger;
}

void
IceProxy::xlog::slice::Dispatcher::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            __del->add(data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::xlog::slice::Dispatcher::addLogData(const ::xlog::slice::LogData& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            __del->addLogData(data, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::xlog::slice::Dispatcher::createZNode(::Ice::Int slot, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            __del->createZNode(slot, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::xlog::slice::Dispatcher::_cpp_register(const ::xlog::slice::LoggerPrx& subscriber, ::Ice::Int frequence, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__Dispatcher__register_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            return __del->_cpp_register(subscriber, frequence, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Int
IceProxy::xlog::slice::Dispatcher::subscribe(const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__Dispatcher__subscribe_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            return __del->subscribe(sub, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Int
IceProxy::xlog::slice::Dispatcher::unsubscribe(const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__Dispatcher__unsubscribe_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            return __del->unsubscribe(sub, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::StringSeq
IceProxy::xlog::slice::Dispatcher::getData(::Ice::Int categoryId, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__Dispatcher__getData_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::Dispatcher* __del = dynamic_cast< ::IceDelegate::xlog::slice::Dispatcher*>(__delBase.get());
            return __del->getData(categoryId, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::xlog::slice::Dispatcher::ice_staticId()
{
    return ::xlog::slice::Dispatcher::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::Dispatcher::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::Dispatcher);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::Dispatcher::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::Dispatcher);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::Dispatcher::__newInstance() const
{
    return new Dispatcher;
}

::Ice::Int
IceProxy::xlog::slice::PublisherService::publish(const ::xlog::slice::LogData& data, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__xlog__slice__PublisherService__publish_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::xlog::slice::PublisherService* __del = dynamic_cast< ::IceDelegate::xlog::slice::PublisherService*>(__delBase.get());
            return __del->publish(data, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::xlog::slice::PublisherService::ice_staticId()
{
    return ::xlog::slice::PublisherService::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::xlog::slice::PublisherService::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::xlog::slice::PublisherService);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::xlog::slice::PublisherService::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::xlog::slice::PublisherService);
}

::IceProxy::Ice::Object*
IceProxy::xlog::slice::PublisherService::__newInstance() const
{
    return new PublisherService;
}

void
IceDelegateM::xlog::slice::Subscriber::notify(const ::Ice::StringSeq& config, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Subscriber__notify_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(config.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            __os->write(&config[0], &config[0] + config.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::xlog::slice::Agent::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Agent__add_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(data.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::xlog::slice::__writeLogDataSeq(__os, &data[0], &data[0] + data.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::xlog::slice::Agent::addFailedLogData(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Agent__addFailedLogData_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(data.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::xlog::slice::__writeLogDataSeq(__os, &data[0], &data[0] + data.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::Ice::StringSeq
IceDelegateM::xlog::slice::Agent::subscribeClient(const ::std::string& prxStr, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Agent__subscribeClient_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(prxStr);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::StringSeq __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::StringSeq
IceDelegateM::xlog::slice::Agent::subscribeSubscriber(const ::Ice::StringSeq& categories, const ::std::string& prxStr, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Agent__subscribeSubscriber_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(categories.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            __os->write(&categories[0], &categories[0] + categories.size());
        }
        __os->write(prxStr);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::StringSeq __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::xlog::slice::Logger::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Logger__add_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(data.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::xlog::slice::__writeLogDataSeq(__os, &data[0], &data[0] + data.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::xlog::slice::Logger::addLogData(const ::xlog::slice::LogData& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Logger__addLogData_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        data.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::xlog::slice::Dispatcher::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__add_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(data.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::xlog::slice::__writeLogDataSeq(__os, &data[0], &data[0] + data.size());
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::xlog::slice::Dispatcher::addLogData(const ::xlog::slice::LogData& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__addLogData_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        data.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::xlog::slice::Dispatcher::createZNode(::Ice::Int slot, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__createZNode_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(slot);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

bool
IceDelegateM::xlog::slice::Dispatcher::_cpp_register(const ::xlog::slice::LoggerPrx& subscriber, ::Ice::Int frequence, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__register_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPrx(::IceInternal::upCast(subscriber.get())));
        __os->write(frequence);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    bool __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::xlog::slice::Dispatcher::subscribe(const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__subscribe_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(sub.get())));
        __os->writePendingObjects();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::xlog::slice::XLogException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::xlog::slice::Dispatcher::unsubscribe(const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__unsubscribe_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(::Ice::ObjectPtr(::IceInternal::upCast(sub.get())));
        __os->writePendingObjects();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::xlog::slice::XLogException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::StringSeq
IceDelegateM::xlog::slice::Dispatcher::getData(::Ice::Int categoryId, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__Dispatcher__getData_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(categoryId);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::StringSeq __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::xlog::slice::XLogException&)
            {
                throw;
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateM::xlog::slice::PublisherService::publish(const ::xlog::slice::LogData& data, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __xlog__slice__PublisherService__publish_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        data.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateD::xlog::slice::Subscriber::notify(const ::Ice::StringSeq& config, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::StringSeq& config, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_config(config)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Subscriber* servant = dynamic_cast< ::xlog::slice::Subscriber*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->notify(_m_config, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::Ice::StringSeq& _m_config;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Subscriber__notify_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(config, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::xlog::slice::Agent::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::xlog::slice::LogDataSeq& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Agent* servant = dynamic_cast< ::xlog::slice::Agent*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->add(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::xlog::slice::LogDataSeq& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Agent__add_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::xlog::slice::Agent::addFailedLogData(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::xlog::slice::LogDataSeq& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Agent* servant = dynamic_cast< ::xlog::slice::Agent*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->addFailedLogData(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::xlog::slice::LogDataSeq& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Agent__addFailedLogData_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::StringSeq
IceDelegateD::xlog::slice::Agent::subscribeClient(const ::std::string& prxStr, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::StringSeq& __result, const ::std::string& prxStr, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_prxStr(prxStr)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Agent* servant = dynamic_cast< ::xlog::slice::Agent*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->subscribeClient(_m_prxStr, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::StringSeq& _result;
        const ::std::string& _m_prxStr;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Agent__subscribeClient_name, ::Ice::Normal, __context);
    ::Ice::StringSeq __result;
    try
    {
        _DirectI __direct(__result, prxStr, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::StringSeq
IceDelegateD::xlog::slice::Agent::subscribeSubscriber(const ::Ice::StringSeq& categories, const ::std::string& prxStr, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::StringSeq& __result, const ::Ice::StringSeq& categories, const ::std::string& prxStr, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_categories(categories),
            _m_prxStr(prxStr)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Agent* servant = dynamic_cast< ::xlog::slice::Agent*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->subscribeSubscriber(_m_categories, _m_prxStr, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::StringSeq& _result;
        const ::Ice::StringSeq& _m_categories;
        const ::std::string& _m_prxStr;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Agent__subscribeSubscriber_name, ::Ice::Normal, __context);
    ::Ice::StringSeq __result;
    try
    {
        _DirectI __direct(__result, categories, prxStr, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::xlog::slice::Logger::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::xlog::slice::LogDataSeq& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Logger* servant = dynamic_cast< ::xlog::slice::Logger*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->add(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::xlog::slice::LogDataSeq& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Logger__add_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::xlog::slice::Logger::addLogData(const ::xlog::slice::LogData& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::xlog::slice::LogData& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Logger* servant = dynamic_cast< ::xlog::slice::Logger*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->addLogData(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::xlog::slice::LogData& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Logger__addLogData_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::xlog::slice::Dispatcher::add(const ::xlog::slice::LogDataSeq& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::xlog::slice::LogDataSeq& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->add(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::xlog::slice::LogDataSeq& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__add_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::xlog::slice::Dispatcher::addLogData(const ::xlog::slice::LogData& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::xlog::slice::LogData& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->addLogData(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::xlog::slice::LogData& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__addLogData_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::xlog::slice::Dispatcher::createZNode(::Ice::Int slot, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int slot, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_slot(slot)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->createZNode(_m_slot, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int _m_slot;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__createZNode_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(slot, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

bool
IceDelegateD::xlog::slice::Dispatcher::_cpp_register(const ::xlog::slice::LoggerPrx& subscriber, ::Ice::Int frequence, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::xlog::slice::LoggerPrx& subscriber, ::Ice::Int frequence, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_subscriber(subscriber),
            _m_frequence(frequence)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->_cpp_register(_m_subscriber, _m_frequence, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::xlog::slice::LoggerPrx& _m_subscriber;
        ::Ice::Int _m_frequence;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__register_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, subscriber, frequence, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Int
IceDelegateD::xlog::slice::Dispatcher::subscribe(const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_sub(sub)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->subscribe(_m_sub, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::xlog::slice::SubscriptionPtr& _m_sub;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__subscribe_name, ::Ice::Normal, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, sub, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::xlog::slice::XLogException&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Int
IceDelegateD::xlog::slice::Dispatcher::unsubscribe(const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::xlog::slice::SubscriptionPtr& sub, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_sub(sub)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->unsubscribe(_m_sub, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::xlog::slice::SubscriptionPtr& _m_sub;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__unsubscribe_name, ::Ice::Normal, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, sub, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::xlog::slice::XLogException&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::StringSeq
IceDelegateD::xlog::slice::Dispatcher::getData(::Ice::Int categoryId, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::StringSeq& __result, ::Ice::Int categoryId, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_categoryId(categoryId)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::Dispatcher* servant = dynamic_cast< ::xlog::slice::Dispatcher*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            try
            {
                _result = servant->getData(_m_categoryId, _current);
                return ::Ice::DispatchOK;
            }
            catch(const ::Ice::UserException& __ex)
            {
                setUserException(__ex);
                return ::Ice::DispatchUserException;
            }
        }
        
    private:
        
        ::Ice::StringSeq& _result;
        ::Ice::Int _m_categoryId;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__Dispatcher__getData_name, ::Ice::Normal, __context);
    ::Ice::StringSeq __result;
    try
    {
        _DirectI __direct(__result, categoryId, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::xlog::slice::XLogException&)
    {
        throw;
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Int
IceDelegateD::xlog::slice::PublisherService::publish(const ::xlog::slice::LogData& data, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::xlog::slice::LogData& data, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_data(data)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::xlog::slice::PublisherService* servant = dynamic_cast< ::xlog::slice::PublisherService*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->publish(_m_data, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
        const ::xlog::slice::LogData& _m_data;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __xlog__slice__PublisherService__publish_name, ::Ice::Normal, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, data, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

xlog::slice::Subscription::Subscription(const ::Ice::StringSeq& __ice_categories, const ::xlog::slice::Str2StrDict& __ice_options) :
    categories(__ice_categories),
    options(__ice_options)
{
}

::Ice::ObjectPtr
xlog::slice::Subscription::ice_clone() const
{
    ::xlog::slice::SubscriptionPtr __p = new ::xlog::slice::Subscription(*this);
    return __p;
}

static const ::std::string __xlog__slice__Subscription_ids[2] =
{
    "::Ice::Object",
    "::xlog::slice::Subscription"
};

bool
xlog::slice::Subscription::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__Subscription_ids, __xlog__slice__Subscription_ids + 2, _s);
}

::std::vector< ::std::string>
xlog::slice::Subscription::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__Subscription_ids[0], &__xlog__slice__Subscription_ids[2]);
}

const ::std::string&
xlog::slice::Subscription::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__Subscription_ids[1];
}

const ::std::string&
xlog::slice::Subscription::ice_staticId()
{
    return __xlog__slice__Subscription_ids[1];
}

void
xlog::slice::Subscription::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    if(categories.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&categories[0], &categories[0] + categories.size());
    }
    ::xlog::slice::__writeStr2StrDict(__os, options);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
xlog::slice::Subscription::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(categories);
    ::xlog::slice::__readStr2StrDict(__is, options);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
xlog::slice::Subscription::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Subscription was not generated with stream support";
    throw ex;
}

void
xlog::slice::Subscription::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Subscription was not generated with stream support";
    throw ex;
}

class __F__xlog__slice__Subscription : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::xlog::slice::Subscription::ice_staticId());
        return new ::xlog::slice::Subscription;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__xlog__slice__Subscription_Ptr = new __F__xlog__slice__Subscription;

const ::Ice::ObjectFactoryPtr&
xlog::slice::Subscription::ice_factory()
{
    return __F__xlog__slice__Subscription_Ptr;
}

class __F__xlog__slice__Subscription__Init
{
public:

    __F__xlog__slice__Subscription__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::xlog::slice::Subscription::ice_staticId(), ::xlog::slice::Subscription::ice_factory());
    }

    ~__F__xlog__slice__Subscription__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::xlog::slice::Subscription::ice_staticId());
    }
};

static __F__xlog__slice__Subscription__Init __F__xlog__slice__Subscription__i;

#ifdef __APPLE__
extern "C" { void __F__xlog__slice__Subscription__initializer() {} }
#endif

void 
xlog::slice::__patch__SubscriptionPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::SubscriptionPtr* p = static_cast< ::xlog::slice::SubscriptionPtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::SubscriptionPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::Subscription::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::Subscription& l, const ::xlog::slice::Subscription& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::Subscription& l, const ::xlog::slice::Subscription& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

xlog::slice::PushSubscription::PushSubscription(const ::Ice::StringSeq& __ice_categories, const ::xlog::slice::Str2StrDict& __ice_options, const ::std::string& __ice_host) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Subscription(__ice_categories, __ice_options)
#else
    ::xlog::slice::Subscription(__ice_categories, __ice_options)
#endif
,
    host(__ice_host)
{
}

::Ice::ObjectPtr
xlog::slice::PushSubscription::ice_clone() const
{
    ::xlog::slice::PushSubscriptionPtr __p = new ::xlog::slice::PushSubscription(*this);
    return __p;
}

static const ::std::string __xlog__slice__PushSubscription_ids[3] =
{
    "::Ice::Object",
    "::xlog::slice::PushSubscription",
    "::xlog::slice::Subscription"
};

bool
xlog::slice::PushSubscription::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__PushSubscription_ids, __xlog__slice__PushSubscription_ids + 3, _s);
}

::std::vector< ::std::string>
xlog::slice::PushSubscription::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__PushSubscription_ids[0], &__xlog__slice__PushSubscription_ids[3]);
}

const ::std::string&
xlog::slice::PushSubscription::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__PushSubscription_ids[1];
}

const ::std::string&
xlog::slice::PushSubscription::ice_staticId()
{
    return __xlog__slice__PushSubscription_ids[1];
}

void
xlog::slice::PushSubscription::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->write(host);
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Subscription::__write(__os);
#else
    ::xlog::slice::Subscription::__write(__os);
#endif
}

void
xlog::slice::PushSubscription::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->read(host);
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Subscription::__read(__is, true);
#else
    ::xlog::slice::Subscription::__read(__is, true);
#endif
}

void
xlog::slice::PushSubscription::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::PushSubscription was not generated with stream support";
    throw ex;
}

void
xlog::slice::PushSubscription::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::PushSubscription was not generated with stream support";
    throw ex;
}

class __F__xlog__slice__PushSubscription : public ::Ice::ObjectFactory
{
public:

    virtual ::Ice::ObjectPtr
    create(const ::std::string& type)
    {
        assert(type == ::xlog::slice::PushSubscription::ice_staticId());
        return new ::xlog::slice::PushSubscription;
    }

    virtual void
    destroy()
    {
    }
};

static ::Ice::ObjectFactoryPtr __F__xlog__slice__PushSubscription_Ptr = new __F__xlog__slice__PushSubscription;

const ::Ice::ObjectFactoryPtr&
xlog::slice::PushSubscription::ice_factory()
{
    return __F__xlog__slice__PushSubscription_Ptr;
}

class __F__xlog__slice__PushSubscription__Init
{
public:

    __F__xlog__slice__PushSubscription__Init()
    {
        ::IceInternal::factoryTable->addObjectFactory(::xlog::slice::PushSubscription::ice_staticId(), ::xlog::slice::PushSubscription::ice_factory());
    }

    ~__F__xlog__slice__PushSubscription__Init()
    {
        ::IceInternal::factoryTable->removeObjectFactory(::xlog::slice::PushSubscription::ice_staticId());
    }
};

static __F__xlog__slice__PushSubscription__Init __F__xlog__slice__PushSubscription__i;

#ifdef __APPLE__
extern "C" { void __F__xlog__slice__PushSubscription__initializer() {} }
#endif

void 
xlog::slice::__patch__PushSubscriptionPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::PushSubscriptionPtr* p = static_cast< ::xlog::slice::PushSubscriptionPtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::PushSubscriptionPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::PushSubscription::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::PushSubscription& l, const ::xlog::slice::PushSubscription& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::PushSubscription& l, const ::xlog::slice::PushSubscription& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
xlog::slice::Subscriber::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __xlog__slice__Subscriber_ids[2] =
{
    "::Ice::Object",
    "::xlog::slice::Subscriber"
};

bool
xlog::slice::Subscriber::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__Subscriber_ids, __xlog__slice__Subscriber_ids + 2, _s);
}

::std::vector< ::std::string>
xlog::slice::Subscriber::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__Subscriber_ids[0], &__xlog__slice__Subscriber_ids[2]);
}

const ::std::string&
xlog::slice::Subscriber::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__Subscriber_ids[1];
}

const ::std::string&
xlog::slice::Subscriber::ice_staticId()
{
    return __xlog__slice__Subscriber_ids[1];
}

::Ice::DispatchStatus
xlog::slice::Subscriber::___notify(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::StringSeq config;
    __is->read(config);
    __is->endReadEncaps();
    notify(config, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __xlog__slice__Subscriber_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "notify"
};

::Ice::DispatchStatus
xlog::slice::Subscriber::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__xlog__slice__Subscriber_all, __xlog__slice__Subscriber_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __xlog__slice__Subscriber_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___notify(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
xlog::slice::Subscriber::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
xlog::slice::Subscriber::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
xlog::slice::Subscriber::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Subscriber was not generated with stream support";
    throw ex;
}

void
xlog::slice::Subscriber::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Subscriber was not generated with stream support";
    throw ex;
}

void 
xlog::slice::__patch__SubscriberPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::SubscriberPtr* p = static_cast< ::xlog::slice::SubscriberPtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::SubscriberPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::Subscriber::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::Subscriber& l, const ::xlog::slice::Subscriber& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::Subscriber& l, const ::xlog::slice::Subscriber& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
xlog::slice::Agent::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __xlog__slice__Agent_ids[2] =
{
    "::Ice::Object",
    "::xlog::slice::Agent"
};

bool
xlog::slice::Agent::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__Agent_ids, __xlog__slice__Agent_ids + 2, _s);
}

::std::vector< ::std::string>
xlog::slice::Agent::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__Agent_ids[0], &__xlog__slice__Agent_ids[2]);
}

const ::std::string&
xlog::slice::Agent::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__Agent_ids[1];
}

const ::std::string&
xlog::slice::Agent::ice_staticId()
{
    return __xlog__slice__Agent_ids[1];
}

::Ice::DispatchStatus
xlog::slice::Agent::___add(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogDataSeq data;
    ::xlog::slice::__readLogDataSeq(__is, data);
    __is->endReadEncaps();
    add(data, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Agent::___addFailedLogData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogDataSeq data;
    ::xlog::slice::__readLogDataSeq(__is, data);
    __is->endReadEncaps();
    addFailedLogData(data, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Agent::___subscribeClient(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string prxStr;
    __is->read(prxStr);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::StringSeq __ret = subscribeClient(prxStr, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Agent::___subscribeSubscriber(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::StringSeq categories;
    ::std::string prxStr;
    __is->read(categories);
    __is->read(prxStr);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::StringSeq __ret = subscribeSubscriber(categories, prxStr, __current);
    if(__ret.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&__ret[0], &__ret[0] + __ret.size());
    }
    return ::Ice::DispatchOK;
}

static ::std::string __xlog__slice__Agent_all[] =
{
    "add",
    "addFailedLogData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "subscribeClient",
    "subscribeSubscriber"
};

::Ice::DispatchStatus
xlog::slice::Agent::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__xlog__slice__Agent_all, __xlog__slice__Agent_all + 8, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __xlog__slice__Agent_all)
    {
        case 0:
        {
            return ___add(in, current);
        }
        case 1:
        {
            return ___addFailedLogData(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
        case 6:
        {
            return ___subscribeClient(in, current);
        }
        case 7:
        {
            return ___subscribeSubscriber(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
xlog::slice::Agent::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
xlog::slice::Agent::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
xlog::slice::Agent::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Agent was not generated with stream support";
    throw ex;
}

void
xlog::slice::Agent::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Agent was not generated with stream support";
    throw ex;
}

void 
xlog::slice::__patch__AgentPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::AgentPtr* p = static_cast< ::xlog::slice::AgentPtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::AgentPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::Agent::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::Agent& l, const ::xlog::slice::Agent& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::Agent& l, const ::xlog::slice::Agent& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
xlog::slice::Logger::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __xlog__slice__Logger_ids[2] =
{
    "::Ice::Object",
    "::xlog::slice::Logger"
};

bool
xlog::slice::Logger::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__Logger_ids, __xlog__slice__Logger_ids + 2, _s);
}

::std::vector< ::std::string>
xlog::slice::Logger::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__Logger_ids[0], &__xlog__slice__Logger_ids[2]);
}

const ::std::string&
xlog::slice::Logger::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__Logger_ids[1];
}

const ::std::string&
xlog::slice::Logger::ice_staticId()
{
    return __xlog__slice__Logger_ids[1];
}

::Ice::DispatchStatus
xlog::slice::Logger::___add(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogDataSeq data;
    ::xlog::slice::__readLogDataSeq(__is, data);
    __is->endReadEncaps();
    add(data, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Logger::___addLogData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogData data;
    data.__read(__is);
    __is->endReadEncaps();
    addLogData(data, __current);
    return ::Ice::DispatchOK;
}

static ::std::string __xlog__slice__Logger_all[] =
{
    "add",
    "addLogData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
xlog::slice::Logger::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__xlog__slice__Logger_all, __xlog__slice__Logger_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __xlog__slice__Logger_all)
    {
        case 0:
        {
            return ___add(in, current);
        }
        case 1:
        {
            return ___addLogData(in, current);
        }
        case 2:
        {
            return ___ice_id(in, current);
        }
        case 3:
        {
            return ___ice_ids(in, current);
        }
        case 4:
        {
            return ___ice_isA(in, current);
        }
        case 5:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
xlog::slice::Logger::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
xlog::slice::Logger::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
xlog::slice::Logger::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Logger was not generated with stream support";
    throw ex;
}

void
xlog::slice::Logger::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Logger was not generated with stream support";
    throw ex;
}

void 
xlog::slice::__patch__LoggerPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::LoggerPtr* p = static_cast< ::xlog::slice::LoggerPtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::LoggerPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::Logger::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::Logger& l, const ::xlog::slice::Logger& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::Logger& l, const ::xlog::slice::Logger& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
xlog::slice::Dispatcher::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __xlog__slice__Dispatcher_ids[2] =
{
    "::Ice::Object",
    "::xlog::slice::Dispatcher"
};

bool
xlog::slice::Dispatcher::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__Dispatcher_ids, __xlog__slice__Dispatcher_ids + 2, _s);
}

::std::vector< ::std::string>
xlog::slice::Dispatcher::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__Dispatcher_ids[0], &__xlog__slice__Dispatcher_ids[2]);
}

const ::std::string&
xlog::slice::Dispatcher::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__Dispatcher_ids[1];
}

const ::std::string&
xlog::slice::Dispatcher::ice_staticId()
{
    return __xlog__slice__Dispatcher_ids[1];
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___add(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogDataSeq data;
    ::xlog::slice::__readLogDataSeq(__is, data);
    __is->endReadEncaps();
    add(data, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___addLogData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogData data;
    data.__read(__is);
    __is->endReadEncaps();
    addLogData(data, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___createZNode(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Int slot;
    __is->read(slot);
    __is->endReadEncaps();
    createZNode(slot, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___register(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LoggerPrx subscriber;
    ::Ice::Int frequence;
    ::xlog::slice::__read(__is, subscriber);
    __is->read(frequence);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = _cpp_register(subscriber, frequence, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___subscribe(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::SubscriptionPtr sub;
    __is->read(::xlog::slice::__patch__SubscriptionPtr, &sub);
    __is->readPendingObjects();
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::Ice::Int __ret = subscribe(sub, __current);
        __os->write(__ret);
    }
    catch(const ::xlog::slice::XLogException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___unsubscribe(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::SubscriptionPtr sub;
    __is->read(::xlog::slice::__patch__SubscriptionPtr, &sub);
    __is->readPendingObjects();
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::Ice::Int __ret = unsubscribe(sub, __current);
        __os->write(__ret);
    }
    catch(const ::xlog::slice::XLogException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
xlog::slice::Dispatcher::___getData(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Int categoryId;
    __is->read(categoryId);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    try
    {
        ::Ice::StringSeq __ret = getData(categoryId, __current);
        if(__ret.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            __os->write(&__ret[0], &__ret[0] + __ret.size());
        }
    }
    catch(const ::xlog::slice::XLogException& __ex)
    {
        __os->write(__ex);
        return ::Ice::DispatchUserException;
    }
    return ::Ice::DispatchOK;
}

static ::std::string __xlog__slice__Dispatcher_all[] =
{
    "add",
    "addLogData",
    "createZNode",
    "getData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "register",
    "subscribe",
    "unsubscribe"
};

::Ice::DispatchStatus
xlog::slice::Dispatcher::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__xlog__slice__Dispatcher_all, __xlog__slice__Dispatcher_all + 11, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __xlog__slice__Dispatcher_all)
    {
        case 0:
        {
            return ___add(in, current);
        }
        case 1:
        {
            return ___addLogData(in, current);
        }
        case 2:
        {
            return ___createZNode(in, current);
        }
        case 3:
        {
            return ___getData(in, current);
        }
        case 4:
        {
            return ___ice_id(in, current);
        }
        case 5:
        {
            return ___ice_ids(in, current);
        }
        case 6:
        {
            return ___ice_isA(in, current);
        }
        case 7:
        {
            return ___ice_ping(in, current);
        }
        case 8:
        {
            return ___register(in, current);
        }
        case 9:
        {
            return ___subscribe(in, current);
        }
        case 10:
        {
            return ___unsubscribe(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
xlog::slice::Dispatcher::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
xlog::slice::Dispatcher::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
xlog::slice::Dispatcher::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Dispatcher was not generated with stream support";
    throw ex;
}

void
xlog::slice::Dispatcher::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::Dispatcher was not generated with stream support";
    throw ex;
}

void 
xlog::slice::__patch__DispatcherPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::DispatcherPtr* p = static_cast< ::xlog::slice::DispatcherPtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::DispatcherPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::Dispatcher::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::Dispatcher& l, const ::xlog::slice::Dispatcher& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::Dispatcher& l, const ::xlog::slice::Dispatcher& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

::Ice::ObjectPtr
xlog::slice::PublisherService::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __xlog__slice__PublisherService_ids[2] =
{
    "::Ice::Object",
    "::xlog::slice::PublisherService"
};

bool
xlog::slice::PublisherService::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__xlog__slice__PublisherService_ids, __xlog__slice__PublisherService_ids + 2, _s);
}

::std::vector< ::std::string>
xlog::slice::PublisherService::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__xlog__slice__PublisherService_ids[0], &__xlog__slice__PublisherService_ids[2]);
}

const ::std::string&
xlog::slice::PublisherService::ice_id(const ::Ice::Current&) const
{
    return __xlog__slice__PublisherService_ids[1];
}

const ::std::string&
xlog::slice::PublisherService::ice_staticId()
{
    return __xlog__slice__PublisherService_ids[1];
}

::Ice::DispatchStatus
xlog::slice::PublisherService::___publish(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::xlog::slice::LogData data;
    data.__read(__is);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = publish(data, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

static ::std::string __xlog__slice__PublisherService_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "publish"
};

::Ice::DispatchStatus
xlog::slice::PublisherService::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__xlog__slice__PublisherService_all, __xlog__slice__PublisherService_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __xlog__slice__PublisherService_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___publish(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
xlog::slice::PublisherService::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
xlog::slice::PublisherService::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
xlog::slice::PublisherService::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::PublisherService was not generated with stream support";
    throw ex;
}

void
xlog::slice::PublisherService::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type xlog::slice::PublisherService was not generated with stream support";
    throw ex;
}

void 
xlog::slice::__patch__PublisherServicePtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::xlog::slice::PublisherServicePtr* p = static_cast< ::xlog::slice::PublisherServicePtr*>(__addr);
    assert(p);
    *p = ::xlog::slice::PublisherServicePtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::xlog::slice::PublisherService::ice_staticId(), v->ice_id());
    }
}

bool
xlog::slice::operator==(const ::xlog::slice::PublisherService& l, const ::xlog::slice::PublisherService& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
xlog::slice::operator<(const ::xlog::slice::PublisherService& l, const ::xlog::slice::PublisherService& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
