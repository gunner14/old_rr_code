/**
 * $Id: WUserRightAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.control.BlockException;
import com.xiaonei.platform.core.model.UserRight;
import com.xiaonei.platform.core.model.factory.WUserRightFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserRightAdapter;
import com.xiaonei.xce.userright.UserRightAdapter;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserRightAdapter extends BaseUsersAdapter<UserRightAdapter<UserRight>> implements
        IUserRightAdapter {

    @Override
    protected UserRightAdapter<UserRight> makeObject() {
        return new UserRightAdapter<UserRight>(new WUserRightFactory());
    }
    @Override
    public void createUserRight(int userId, UserRight userRight) {
        getAdapter().createUserRight(userId, userRight);
        
    }
    @Override
    public UserRight getUserRight(int userId) {
        try {
            return getAdapter().getUserRight(userId);
        } catch (Exception e) {
            throw new BlockException(e);
        }
    }
    @Override
    public void setUserRight(int userId, UserRight userRight) {
        getAdapter().setUserRight(userId, userRight);
    }
    protected WUserRightAdapter() {
    }
    protected String getServiceName() {
        return "userright";
    }
}
