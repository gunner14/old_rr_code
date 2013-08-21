/**
 * $Id: WUserBornAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.sns.platform.core.opt.ice.IUserBornAdapter;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;
import com.xiaonei.sns.platform.core.opt.ice.model.factory.WUserBornFactory;
import com.xiaonei.xce.userborn.UserBornAdapter;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-20
 */
public class WUserBornAdapter extends BaseUsersAdapter<UserBornAdapter<UserBorn>> implements
        IUserBornAdapter {

    @Override
    public void createUserBorn(int userId, UserBorn userBorn) {
        checkUserId(userId);
        getAdapter().createUserBorn(userId, userBorn);
    }

    @Override
    public UserBorn getUserBorn(int userId) {
        checkUserId(userId);
        UserBorn ret = null;
        try {
            ret = getAdapter().getUserBorn(userId);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }
        return ret;
    }

    @Override
    public void setUserBorn(int userId, UserBorn userBorn) {
        checkUserId(userId);
        getAdapter().setUserBorn(userId, userBorn);
    }

    @Override
    protected UserBornAdapter<UserBorn> makeObject() {
        return new UserBornAdapter<UserBorn>(new WUserBornFactory());
    }
    protected String getServiceName() {
        return "userborn";
    }
}
