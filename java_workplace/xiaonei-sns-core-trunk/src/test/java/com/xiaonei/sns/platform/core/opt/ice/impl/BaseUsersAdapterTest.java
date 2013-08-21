/**
 * $Id: BaseUsersAdapterTest.java 16545 2010-02-05 11:23:43Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.Date;

import junit.framework.Assert;

import org.junit.Test;

import com.xiaonei.platform.core.control.BlockException;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class BaseUsersAdapterTest {
    BaseUsersAdapter<Date> adapter = new BaseUsersAdapter<Date>() {
        @Override
        protected Date makeObject() {
            return new Date();
        }
        @Override
        protected String getServiceName() {
            return "test";
        }
    };
    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.BaseUsersAdapter#checkUserId(int[])}.
     */
    @Test
    public void testCheckUserId() throws Exception{
        try {
            adapter.checkUserId(-1);
            throw new Exception();
        } catch (BlockException e) {
           //ignore
        }
        
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.BaseUsersAdapter#rethrowBlockException(java.lang.Exception, int)}.
     */
    @Test
    public void testRethrowBlockException() {
        try {
            adapter.rethrowBlockException(new Exception("test"), 10);
        } catch (BlockException e) {
            //ignore
        }
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.BaseUsersAdapter#getAdapter()}.
     */
    @Test
    public void testGetAdapter() {
       Assert.assertSame(adapter.getAdapter(), adapter.getAdapter());
    }

}
