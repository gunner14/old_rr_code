/**
 * $Id: WUserLoginCountAdapter.java 16577 2010-02-09 01:58:35Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.sns.platform.core.opt.ice.IUserLoginCountAdapter;
import com.xiaonei.xce.userlogincount.UserLoginCountAdapter;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserLoginCountAdapter extends BaseUsersAdapter<UserLoginCountAdapter> implements
        IUserLoginCountAdapter {

    protected WUserLoginCountAdapter() {
    }

    @Override
    public int getUserLoginCount(int userId) {
        int cnt = -1;
        try {
            cnt = getAdapter().getUserLoginCount(userId);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }
        return cnt;
    }

    @Override
    public void incUserLoginCount(int userid) {
        getAdapter().incUserLoginCount(userid);
    }

    @Override
    protected UserLoginCountAdapter makeObject() {
        return new UserLoginCountAdapter();
    }

    @Override
    protected String getServiceName() {
        return "userlogincount";
    }
}
