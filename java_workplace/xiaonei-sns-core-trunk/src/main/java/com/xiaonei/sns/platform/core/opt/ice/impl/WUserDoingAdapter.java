/**
 * $Id: WUserDoingAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.sns.platform.core.opt.ice.IUserDoingAdapter;
import com.xiaonei.sns.platform.core.opt.ice.model.UserDoing;
import com.xiaonei.sns.platform.core.opt.ice.model.factory.WUserDoingFactory;
import com.xiaonei.xce.userdoing.UserDoingAdapter;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-20
 */
public class WUserDoingAdapter extends BaseUsersAdapter<UserDoingAdapter<UserDoing>> implements
        IUserDoingAdapter {

    @Override
    protected UserDoingAdapter<UserDoing> makeObject() {
        return new UserDoingAdapter<UserDoing>(new WUserDoingFactory());
    }

    @Override
    public void createUserDoing(int userId, UserDoing userDoing) {
        checkUserId(userId);
        getAdapter().createUserDoing(userId, userDoing);
    }

    @Override
    public UserDoing getUserDoing(int userId) {
        checkUserId(userId);
        UserDoing ret = null;
        try {
            ret = getAdapter().getUserDoing(userId);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }
        return ret;
    }

    @Override
    public void setUserDoing(int userId, UserDoing userDoing) {
        checkUserId(userId);
        getAdapter().setUserDoing(userId, userDoing);
    }
    protected String getServiceName() {
        return "userdoing";
    }
}
