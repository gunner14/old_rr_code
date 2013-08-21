// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1

package echo;

public interface _EchoManagerDel extends Ice._ObjectDel
{
    int echo(int count, int delay, java.util.Map<String, String> __ctx)
        throws IceInternal.LocalExceptionWrapper;
}
