package com.xiaonei.sns.platform.core.opt.ice.impl;

import mop.hi.oce.adapter.impl.UserConfigInfoAdapterImpl;

import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.model.factory.WUserConfigInfoFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserConfigInfoAdapter;

public class WUserConfigInfoAdapter extends BaseUsersAdapter<UserConfigInfoAdapterImpl> implements
        IUserConfigInfoAdapter {

    @Override
    protected UserConfigInfoAdapterImpl makeObject() {
        return new UserConfigInfoAdapterImpl(new WUserConfigInfoFactory());
    }

    @Override
    public void createUserConfig(int userId, UserConfigInfo uc) {
        checkUserId(userId);
        getAdapter().createUserConfig(userId, uc);
    }

    /**
     * @deprecated
     * @see {@linkplain SnsAdapterFactory#getUserConfigInfoAdapter()}
     */
    public static IUserConfigInfoAdapter getInstance() {
        return SnsAdapterFactory.getUserConfigInfoAdapter();
    }

    WUserConfigInfoAdapter() {
    }

    public UserConfigInfo getUserConfig(int userId) {
        checkUserId(userId);
        try {
            return (UserConfigInfo) getAdapter().getUserConfig(userId);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }
        return null;
    }

    public void setUserConfig(int userId, UserConfigInfo uc) {
        checkUserId(userId);
        try {
            getAdapter().setUserConfig(userId, uc);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }
    }
    protected String getServiceName() {
        return "userconfiginfo";
    }
}
