/**
 * $Id: AbstractLogBlockable.java 14879 2010-01-14 08:46:39Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.platform.core.control;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-14
 */
public abstract class AbstractLogBlockable implements ILogBlockable {

    protected static final Log log = LogFactory.getLog(AbstractLogBlockable.class);

    protected int timeout = 0;

    protected String name;

    @Override
    public String getName() {
        return this.name;
    }

    @Override
    public void setTimeout(int timeout) {
        this.timeout = timeout;
    }

    public int getTimeout() {
        return timeout;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public Object call() throws Exception {
        Object ret = null;
        long start=System.currentTimeMillis();
        try {
            ret=execute();
        } finally {
            long cost = System.currentTimeMillis() - start;
            if (timeout > 0 && cost > timeout) {
                logTimeout(cost,timeout);
            }
        }
        return ret;
    }
    
    public static Log getLog() {
        return log;
    }

    /**
     * log the message when timeout
     */
    protected abstract void logTimeout(long costTime,long expectTime);

    protected abstract Object execute() throws Exception;
}
