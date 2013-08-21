/**
 * $Id: WUserTimeAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.platform.core.model.factory.WUserTimeFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserTimeAdapter;
import com.xiaonei.xce.usertime.UserTimeAdapter;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-2-5
 */
public class WUserTimeAdapter extends BaseUsersAdapter<UserTimeAdapter<UserTime>> implements IUserTimeAdapter {

    @Override
    protected String getServiceName() {
        return "usertime";
    }
    @Override
    protected UserTimeAdapter<UserTime> makeObject() {
        return new UserTimeAdapter<UserTime>(new WUserTimeFactory());
    }
    @Override
    public void createUserTime(int id, UserTime userTime) {
        getAdapter().createUserTime(id, userTime);

    }

    @Override
    public UserTime getUserTime(int id) {
        return getAdapter().getUserTime(id);
    }

    @Override
    public void setUserTime(int id, UserTime userTime) {
       getAdapter().setUserTime(id, userTime);
    }

}
