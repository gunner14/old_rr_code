/**
 * $Id: WUserNameAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.control.BlockException;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.platform.core.model.factory.WUserNameFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserNameAdapter;
import com.xiaonei.xce.username.UserNameAdapter;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserNameAdapter extends BaseUsersAdapter<UserNameAdapter<UserName>> implements IUserNameAdapter {

    protected WUserNameAdapter() {
    }
    @Override
    public void createUserName(int userId, UserName userName) {
        getAdapter().createUserName(userId, userName);
    }

    @Override
    public UserName getUserName(int userId) {
        try {
            return getAdapter().getUserName(userId);
        } catch (Exception e) {
            throw new BlockException(e);
        }
    }

    @Override
    public void setUserName(int userid, UserName userName) {
        getAdapter().setUserName(userid, userName);
    }
    @Override
    protected UserNameAdapter<UserName> makeObject() {
        return new UserNameAdapter<UserName>(new WUserNameFactory());
    }
    protected String getServiceName() {
        return "username";
    }
}
