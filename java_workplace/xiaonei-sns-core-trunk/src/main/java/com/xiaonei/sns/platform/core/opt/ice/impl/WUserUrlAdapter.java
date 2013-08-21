/**
 * $Id: WUserUrlAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserUrl;
import com.xiaonei.platform.core.model.factory.WUserUrlFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserUrlAdapter;
import com.xiaonei.xce.userurl.UserUrlAdapter;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserUrlAdapter extends BaseUsersAdapter<UserUrlAdapter<UserUrl>> implements IUserUrlAdapter {

    @Override
    protected UserUrlAdapter<UserUrl> makeObject() {
        return new UserUrlAdapter<UserUrl>(new WUserUrlFactory());
    }
    @Override
    public void createUserUrl(int id, UserUrl userUrl) {
        checkUserId(id);
        getAdapter().createUserUrl(id, userUrl);
    }

    @Override
    public UserUrl getUserUrl(int id) {
        checkUserId(id);
        return getAdapter().getUserUrl(id);
    }

    @Override
    public void reload(int id) {
        checkUserId(id);
        getAdapter().reload(id);
    }

    @Override
    public void setUserUrl(int id, UserUrl userUrl) {
        checkUserId(id);
        getAdapter().setUserUrl(id, userUrl);
    }
    protected String getServiceName() {
        return "userurl";
    }
}
