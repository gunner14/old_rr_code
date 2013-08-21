package com.xiaonei.sns.platform.core.opt.ice.impl;

import javax.lang.model.type.NullType;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserHomeView;
import com.xiaonei.sns.platform.core.opt.ice.IUserViewAdapter;

/**
 * @deprecated 由于中间层没有一个UserHomeView服务，所以这里直接去掉了。
 */
public class WUserViewAdapter extends BaseUsersAdapter<NullType> implements IUserViewAdapter {

    protected WUserViewAdapter() {
    }

    /**
     * @deprecated
     * @see {@linkplain SnsAdapterFactory#getUserViewAdapter()}
     */
    public static IUserViewAdapter getInstance() {
        return SnsAdapterFactory.getUserViewAdapter();
    }

    public UserHomeView getUserHomeView(int userId) {
        checkUserId(userId);
        UserHomeView homeView = new UserHomeView();
        try {
            User user = SnsAdapterFactory.getUserAdapter().get(userId);
            if (user == null) return null;
            homeView.setUser(user);

            UserBasic userBasic = SnsAdapterFactory.getUserBasicAdapter().get(userId);
            if (userBasic == null) return null;
            homeView.setUserbasic(userBasic);

            UserDesc userDesc = SnsAdapterFactory.getUserDescAdapter().get(userId);
            if (userDesc == null) return null;
            homeView.setUserdesc(userDesc);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }

        return homeView;
    }

    @Override
    protected NullType makeObject() {
        return null;
    }

    protected String getServiceName() {
        return "userview";
    }
}
