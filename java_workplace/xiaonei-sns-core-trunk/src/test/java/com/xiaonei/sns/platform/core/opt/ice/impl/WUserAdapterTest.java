/**
 * $Id: WUserAdapterTest.java 15288 2010-01-19 07:26:11Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import junit.framework.Assert;

import org.junit.Test;

import com.xiaonei.platform.core.model.User;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserAdapterTest {

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#get(int)}.
     */
    @Test
    public void testGet() {
        User u = SnsAdapterFactory.getUserAdapter().get(285354037);
        Assert.assertEquals(285354037, u.getId());
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getByEmail(java.lang.String)}.
     */
    @Test
    public void testGetByEmail() {
        User u = SnsAdapterFactory.getUserAdapter().getByEmail("xylz");
        Assert.assertEquals("xylz", u.getUserPassportInfo().getAccount());
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getLoginCountAdapter()}.
     */
    @Test
    public void testGetLoginCountAdapter() {
        Assert.assertTrue(SnsAdapterFactory.getUserLoginCountAdapter().getUserLoginCount(285354037)>0);
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getPassportAdapter()}.
     */
    @Test
    public void testGetPassportAdapter() {
        Assert.assertEquals(285354037,SnsAdapterFactory.getUserPassportAdapter().getUserPassport(285354037).getId());
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getRightAdapter()}.
     */
    @Test
    public void testGetRightAdapter() {
        Assert.assertEquals(285354037,SnsAdapterFactory.getUserRightAdapter().getUserRight(285354037).getId());
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getNameAdapter()}.
     */
    @Test
    public void testGetNameAdapter() {
        Assert.assertEquals(285354037,SnsAdapterFactory.getUserNameAdapter().getUserName(285354037).getId());
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getStateAdapter()}.
     */
    @Test
    public void testGetStateAdapter() {
        Assert.assertEquals(285354037,SnsAdapterFactory.getUserStateAdapter().getUserState(285354037).getId());
    }

    /**
     * Test method for {@link com.xiaonei.sns.platform.core.opt.ice.impl.WUserAdapter#getUrlAdapter()}.
     */
    @Test
    public void testGetUrlAdapter() {
        Assert.assertEquals(285354037,SnsAdapterFactory.getUserUrlAdapter().getUserUrl(285354037).getId());
    }

}
