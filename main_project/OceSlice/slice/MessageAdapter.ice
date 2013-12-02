// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
//
//
// **********************************************************************

#ifndef INVOKE_ICE
#define INVOKE_ICE

module renren {
module com {
module message {
module client {


struct MessageVO
{
    int senderId;
    int toId;
    int sessionId;
    string content;
    int sourceType;
    long attachmentKey;
    int attachmentType;
    string attachmentInfo;
    string smallImageUrl;
    string bigImageUrl;
};


interface MessageAdapter
{
   void saveMessge(MessageVO msg);
};

};
};
};
};

#endif
