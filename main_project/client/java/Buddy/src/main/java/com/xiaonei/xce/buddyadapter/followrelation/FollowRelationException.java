package com.xiaonei.xce.buddyadapter.followrelation;


public class FollowRelationException extends Exception {

    private static final long serialVersionUID = 1L;

    public FollowRelationException(String msg) {
        super(msg);
    }

    public FollowRelationException(String msg, Throwable t) {
        super(msg, t);
    }

    public FollowRelationException(Throwable cause) {
        super(cause);
    }
}
