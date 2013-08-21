/**
 * $Id: WUserPassportAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.control.BlockException;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.model.factory.WUserPassportFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserPassportAdapter;
import com.xiaonei.xce.userpassport.NoSuchUserByEmailException;
import com.xiaonei.xce.userpassport.UserPassportAdapter;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserPassportAdapter extends BaseUsersAdapter<UserPassportAdapter<UserPassport>> implements IUserPassportAdapter {

    @Override
    protected UserPassportAdapter<UserPassport> makeObject() {
        return new UserPassportAdapter<UserPassport>(new WUserPassportFactory());
    }

    @Override
    public void createUserPassport(int userId, UserPassport userPassport) {
        getAdapter().createUserPassport(userId, userPassport);
    }

    @Override
    public int getUserIdByEmail(String email) {
        try {
            return getAdapter().getUserIdByEmail(email);
        } catch (NoSuchUserByEmailException e) {
            throw new BlockException(e);
        }
    }

    @Override
    public UserPassport getUserPassport(int userId) {
        try {
            return getAdapter().getUserPassport(userId);
        } catch (Exception e) {
            throw new BlockException(e);
        }
    }

    @Override
    public void setUserPassport(int userid, UserPassport userPassport) {
       getAdapter().setUserPassport(userid, userPassport);
    }
    protected String getServiceName() {
        return "userpassport";
    }
}
