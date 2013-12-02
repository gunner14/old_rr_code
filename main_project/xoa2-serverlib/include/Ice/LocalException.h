// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `LocalException.ice'

#ifndef __Ice_LocalException_h__
#define __Ice_LocalException_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Identity.h>
#include <Ice/BuiltinSequences.h>
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

class ICE_API InitializationException : public ::Ice::LocalException
{
public:

    InitializationException(const char*, int);
    InitializationException(const char*, int, const ::std::string&);
    virtual ~InitializationException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string reason;
};

class ICE_API PluginInitializationException : public ::Ice::LocalException
{
public:

    PluginInitializationException(const char*, int);
    PluginInitializationException(const char*, int, const ::std::string&);
    virtual ~PluginInitializationException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string reason;
};

class ICE_API CollocationOptimizationException : public ::Ice::LocalException
{
public:

    CollocationOptimizationException(const char*, int);
    virtual ~CollocationOptimizationException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API AlreadyRegisteredException : public ::Ice::LocalException
{
public:

    AlreadyRegisteredException(const char*, int);
    AlreadyRegisteredException(const char*, int, const ::std::string&, const ::std::string&);
    virtual ~AlreadyRegisteredException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string kindOfObject;
    ::std::string id;
};

class ICE_API NotRegisteredException : public ::Ice::LocalException
{
public:

    NotRegisteredException(const char*, int);
    NotRegisteredException(const char*, int, const ::std::string&, const ::std::string&);
    virtual ~NotRegisteredException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string kindOfObject;
    ::std::string id;
};

class ICE_API TwowayOnlyException : public ::Ice::LocalException
{
public:

    TwowayOnlyException(const char*, int);
    TwowayOnlyException(const char*, int, const ::std::string&);
    virtual ~TwowayOnlyException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string operation;
};

class ICE_API CloneNotImplementedException : public ::Ice::LocalException
{
public:

    CloneNotImplementedException(const char*, int);
    virtual ~CloneNotImplementedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API UnknownException : public ::Ice::LocalException
{
public:

    UnknownException(const char*, int);
    UnknownException(const char*, int, const ::std::string&);
    virtual ~UnknownException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string unknown;
};

class ICE_API UnknownLocalException : public ::Ice::UnknownException
{
public:

    UnknownLocalException(const char*, int);
    UnknownLocalException(const char*, int, const ::std::string&);
    virtual ~UnknownLocalException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API UnknownUserException : public ::Ice::UnknownException
{
public:

    UnknownUserException(const char*, int);
    UnknownUserException(const char*, int, const ::std::string&);
    virtual ~UnknownUserException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API VersionMismatchException : public ::Ice::LocalException
{
public:

    VersionMismatchException(const char*, int);
    virtual ~VersionMismatchException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API CommunicatorDestroyedException : public ::Ice::LocalException
{
public:

    CommunicatorDestroyedException(const char*, int);
    virtual ~CommunicatorDestroyedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ObjectAdapterDeactivatedException : public ::Ice::LocalException
{
public:

    ObjectAdapterDeactivatedException(const char*, int);
    ObjectAdapterDeactivatedException(const char*, int, const ::std::string&);
    virtual ~ObjectAdapterDeactivatedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string name;
};

class ICE_API ObjectAdapterIdInUseException : public ::Ice::LocalException
{
public:

    ObjectAdapterIdInUseException(const char*, int);
    ObjectAdapterIdInUseException(const char*, int, const ::std::string&);
    virtual ~ObjectAdapterIdInUseException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string id;
};

class ICE_API NoEndpointException : public ::Ice::LocalException
{
public:

    NoEndpointException(const char*, int);
    NoEndpointException(const char*, int, const ::std::string&);
    virtual ~NoEndpointException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string proxy;
};

class ICE_API EndpointParseException : public ::Ice::LocalException
{
public:

    EndpointParseException(const char*, int);
    EndpointParseException(const char*, int, const ::std::string&);
    virtual ~EndpointParseException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string str;
};

class ICE_API EndpointSelectionTypeParseException : public ::Ice::LocalException
{
public:

    EndpointSelectionTypeParseException(const char*, int);
    EndpointSelectionTypeParseException(const char*, int, const ::std::string&);
    virtual ~EndpointSelectionTypeParseException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string str;
};

class ICE_API IdentityParseException : public ::Ice::LocalException
{
public:

    IdentityParseException(const char*, int);
    IdentityParseException(const char*, int, const ::std::string&);
    virtual ~IdentityParseException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string str;
};

class ICE_API ProxyParseException : public ::Ice::LocalException
{
public:

    ProxyParseException(const char*, int);
    ProxyParseException(const char*, int, const ::std::string&);
    virtual ~ProxyParseException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string str;
};

class ICE_API IllegalIdentityException : public ::Ice::LocalException
{
public:

    IllegalIdentityException(const char*, int);
    IllegalIdentityException(const char*, int, const ::Ice::Identity&);
    virtual ~IllegalIdentityException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::Identity id;
};

class ICE_API RequestFailedException : public ::Ice::LocalException
{
public:

    RequestFailedException(const char*, int);
    RequestFailedException(const char*, int, const ::Ice::Identity&, const ::std::string&, const ::std::string&);
    virtual ~RequestFailedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::Identity id;
    ::std::string facet;
    ::std::string operation;
};

class ICE_API ObjectNotExistException : public ::Ice::RequestFailedException
{
public:

    ObjectNotExistException(const char*, int);
    ObjectNotExistException(const char*, int, const ::Ice::Identity&, const ::std::string&, const ::std::string&);
    virtual ~ObjectNotExistException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API FacetNotExistException : public ::Ice::RequestFailedException
{
public:

    FacetNotExistException(const char*, int);
    FacetNotExistException(const char*, int, const ::Ice::Identity&, const ::std::string&, const ::std::string&);
    virtual ~FacetNotExistException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API OperationNotExistException : public ::Ice::RequestFailedException
{
public:

    OperationNotExistException(const char*, int);
    OperationNotExistException(const char*, int, const ::Ice::Identity&, const ::std::string&, const ::std::string&);
    virtual ~OperationNotExistException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API SyscallException : public ::Ice::LocalException
{
public:

    SyscallException(const char*, int);
    SyscallException(const char*, int, ::Ice::Int);
    virtual ~SyscallException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::Int error;
};

class ICE_API SocketException : public ::Ice::SyscallException
{
public:

    SocketException(const char*, int);
    SocketException(const char*, int, ::Ice::Int);
    virtual ~SocketException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API FileException : public ::Ice::SyscallException
{
public:

    FileException(const char*, int);
    FileException(const char*, int, ::Ice::Int, const ::std::string&);
    virtual ~FileException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string path;
};

class ICE_API ConnectFailedException : public ::Ice::SocketException
{
public:

    ConnectFailedException(const char*, int);
    ConnectFailedException(const char*, int, ::Ice::Int);
    virtual ~ConnectFailedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ConnectionRefusedException : public ::Ice::ConnectFailedException
{
public:

    ConnectionRefusedException(const char*, int);
    ConnectionRefusedException(const char*, int, ::Ice::Int);
    virtual ~ConnectionRefusedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ConnectionLostException : public ::Ice::SocketException
{
public:

    ConnectionLostException(const char*, int);
    ConnectionLostException(const char*, int, ::Ice::Int);
    virtual ~ConnectionLostException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API DNSException : public ::Ice::LocalException
{
public:

    DNSException(const char*, int);
    DNSException(const char*, int, ::Ice::Int, const ::std::string&);
    virtual ~DNSException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::Int error;
    ::std::string host;
};

class ICE_API TimeoutException : public ::Ice::LocalException
{
public:

    TimeoutException(const char*, int);
    virtual ~TimeoutException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ConnectTimeoutException : public ::Ice::TimeoutException
{
public:

    ConnectTimeoutException(const char*, int);
    virtual ~ConnectTimeoutException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API CloseTimeoutException : public ::Ice::TimeoutException
{
public:

    CloseTimeoutException(const char*, int);
    virtual ~CloseTimeoutException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ConnectionTimeoutException : public ::Ice::TimeoutException
{
public:

    ConnectionTimeoutException(const char*, int);
    virtual ~ConnectionTimeoutException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ProtocolException : public ::Ice::LocalException
{
public:

    ProtocolException(const char*, int);
    ProtocolException(const char*, int, const ::std::string&);
    virtual ~ProtocolException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string reason;
};

class ICE_API BadMagicException : public ::Ice::ProtocolException
{
public:

    BadMagicException(const char*, int);
    BadMagicException(const char*, int, const ::std::string&, const ::Ice::ByteSeq&);
    virtual ~BadMagicException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::ByteSeq badMagic;
};

class ICE_API UnsupportedProtocolException : public ::Ice::ProtocolException
{
public:

    UnsupportedProtocolException(const char*, int);
    UnsupportedProtocolException(const char*, int, const ::std::string&, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int);
    virtual ~UnsupportedProtocolException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::Int badMajor;
    ::Ice::Int badMinor;
    ::Ice::Int major;
    ::Ice::Int minor;
};

class ICE_API UnsupportedEncodingException : public ::Ice::ProtocolException
{
public:

    UnsupportedEncodingException(const char*, int);
    UnsupportedEncodingException(const char*, int, const ::std::string&, ::Ice::Int, ::Ice::Int, ::Ice::Int, ::Ice::Int);
    virtual ~UnsupportedEncodingException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::Ice::Int badMajor;
    ::Ice::Int badMinor;
    ::Ice::Int major;
    ::Ice::Int minor;
};

class ICE_API UnknownMessageException : public ::Ice::ProtocolException
{
public:

    UnknownMessageException(const char*, int);
    UnknownMessageException(const char*, int, const ::std::string&);
    virtual ~UnknownMessageException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ConnectionNotValidatedException : public ::Ice::ProtocolException
{
public:

    ConnectionNotValidatedException(const char*, int);
    ConnectionNotValidatedException(const char*, int, const ::std::string&);
    virtual ~ConnectionNotValidatedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API UnknownRequestIdException : public ::Ice::ProtocolException
{
public:

    UnknownRequestIdException(const char*, int);
    UnknownRequestIdException(const char*, int, const ::std::string&);
    virtual ~UnknownRequestIdException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API UnknownReplyStatusException : public ::Ice::ProtocolException
{
public:

    UnknownReplyStatusException(const char*, int);
    UnknownReplyStatusException(const char*, int, const ::std::string&);
    virtual ~UnknownReplyStatusException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API CloseConnectionException : public ::Ice::ProtocolException
{
public:

    CloseConnectionException(const char*, int);
    CloseConnectionException(const char*, int, const ::std::string&);
    virtual ~CloseConnectionException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ForcedCloseConnectionException : public ::Ice::ProtocolException
{
public:

    ForcedCloseConnectionException(const char*, int);
    ForcedCloseConnectionException(const char*, int, const ::std::string&);
    virtual ~ForcedCloseConnectionException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API IllegalMessageSizeException : public ::Ice::ProtocolException
{
public:

    IllegalMessageSizeException(const char*, int);
    IllegalMessageSizeException(const char*, int, const ::std::string&);
    virtual ~IllegalMessageSizeException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API CompressionException : public ::Ice::ProtocolException
{
public:

    CompressionException(const char*, int);
    CompressionException(const char*, int, const ::std::string&);
    virtual ~CompressionException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API DatagramLimitException : public ::Ice::ProtocolException
{
public:

    DatagramLimitException(const char*, int);
    DatagramLimitException(const char*, int, const ::std::string&);
    virtual ~DatagramLimitException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API MarshalException : public ::Ice::ProtocolException
{
public:

    MarshalException(const char*, int);
    MarshalException(const char*, int, const ::std::string&);
    virtual ~MarshalException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ProxyUnmarshalException : public ::Ice::MarshalException
{
public:

    ProxyUnmarshalException(const char*, int);
    ProxyUnmarshalException(const char*, int, const ::std::string&);
    virtual ~ProxyUnmarshalException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API UnmarshalOutOfBoundsException : public ::Ice::MarshalException
{
public:

    UnmarshalOutOfBoundsException(const char*, int);
    UnmarshalOutOfBoundsException(const char*, int, const ::std::string&);
    virtual ~UnmarshalOutOfBoundsException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API IllegalIndirectionException : public ::Ice::MarshalException
{
public:

    IllegalIndirectionException(const char*, int);
    IllegalIndirectionException(const char*, int, const ::std::string&);
    virtual ~IllegalIndirectionException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API NoObjectFactoryException : public ::Ice::MarshalException
{
public:

    NoObjectFactoryException(const char*, int);
    NoObjectFactoryException(const char*, int, const ::std::string&, const ::std::string&);
    virtual ~NoObjectFactoryException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string type;
};

class ICE_API UnexpectedObjectException : public ::Ice::MarshalException
{
public:

    UnexpectedObjectException(const char*, int);
    UnexpectedObjectException(const char*, int, const ::std::string&, const ::std::string&, const ::std::string&);
    virtual ~UnexpectedObjectException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string type;
    ::std::string expectedType;
};

class ICE_API MemoryLimitException : public ::Ice::MarshalException
{
public:

    MemoryLimitException(const char*, int);
    MemoryLimitException(const char*, int, const ::std::string&);
    virtual ~MemoryLimitException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API StringConversionException : public ::Ice::MarshalException
{
public:

    StringConversionException(const char*, int);
    StringConversionException(const char*, int, const ::std::string&);
    virtual ~StringConversionException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API EncapsulationException : public ::Ice::MarshalException
{
public:

    EncapsulationException(const char*, int);
    EncapsulationException(const char*, int, const ::std::string&);
    virtual ~EncapsulationException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API NegativeSizeException : public ::Ice::MarshalException
{
public:

    NegativeSizeException(const char*, int);
    NegativeSizeException(const char*, int, const ::std::string&);
    virtual ~NegativeSizeException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API FeatureNotSupportedException : public ::Ice::LocalException
{
public:

    FeatureNotSupportedException(const char*, int);
    FeatureNotSupportedException(const char*, int, const ::std::string&);
    virtual ~FeatureNotSupportedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string unsupportedFeature;
};

class ICE_API SecurityException : public ::Ice::LocalException
{
public:

    SecurityException(const char*, int);
    SecurityException(const char*, int, const ::std::string&);
    virtual ~SecurityException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string reason;
};

class ICE_API FixedProxyException : public ::Ice::LocalException
{
public:

    FixedProxyException(const char*, int);
    virtual ~FixedProxyException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class ICE_API ResponseSentException : public ::Ice::LocalException
{
public:

    ResponseSentException(const char*, int);
    virtual ~ResponseSentException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

}

#endif
