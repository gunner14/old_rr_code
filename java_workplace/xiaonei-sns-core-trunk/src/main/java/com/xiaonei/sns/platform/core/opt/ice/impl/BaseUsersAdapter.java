/**
 * $Id: BaseUsersAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.control.BlockException;
import com.xiaonei.sns.platform.core.opt.ice.XceException;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-12
 */
abstract class BaseUsersAdapter<T> {

    private volatile T adapter;
    protected void checkUserId(int... ids) {
        if (ids == null) return;
        for (int id : ids)
            if (id <= 0) throw new BlockException("error user id:" + id);
    }

    protected void rethrowBlockException(Exception ex, int userId) throws BlockException {
        if (ex instanceof BlockException) throw (BlockException) ex;
        if (ex instanceof Ice.TimeoutException) {
            throw new XceException(getServiceName()+" timeout for id " + userId + ". ", ex);
        }
        if (ex instanceof Ice.UnknownUserException) {
            throw new XceException(getServiceName()+" unkown user for id " + userId + ". ", ex);
        }
        throw new BlockException(getServiceName()+" [userId=" + userId + "]" + ex.getMessage(), ex);
    }

    protected final T getAdapter() {
        if (adapter == null) {
            synchronized (this) {
                if (adapter == null) {
                    adapter = makeObject();
                }
            }
        }
        return adapter;
    }
    /**
     * make the real adapter <br />
     * Normally, this method will be called only once.
     */
    protected abstract T makeObject();
    
    protected abstract String getServiceName();
}
