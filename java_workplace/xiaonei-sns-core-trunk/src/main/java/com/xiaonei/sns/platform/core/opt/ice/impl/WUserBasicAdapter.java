package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.factory.WUserBasicFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserBasicAdapter;
import com.xiaonei.xce.userbasic.UserBasicAdapter;

public class WUserBasicAdapter extends BaseUsersAdapter<UserBasicAdapter<UserBasic>> implements
        IUserBasicAdapter {

    @Override
    protected UserBasicAdapter<UserBasic> makeObject() {
        return new UserBasicAdapter<UserBasic>(new WUserBasicFactory());
    }

    /**
     * @deprecated
     * @see {@linkplain SnsAdapterFactory#getUserBasicAdapter()}
     */
    public static IUserBasicAdapter getInstance() {
        return SnsAdapterFactory.getUserBasicAdapter();
    }

    WUserBasicAdapter() {
    }

    public UserBasic get(int id) {
        checkUserId(id);
        try {
            return getAdapter().getUserBasic(id);
        } catch (Exception e) {
            rethrowBlockException(e, id);
        }
        return null;
    }
    protected String getServiceName() {
        return "userbasic";
    }
}
