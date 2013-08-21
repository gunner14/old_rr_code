/**
 * $Id: WUserFavAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserFav;
import com.xiaonei.platform.core.model.factory.WUserFavFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserFavAdapter;
import com.xiaonei.xce.userfav.UserFavAdapter;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-2-5
 */
public class WUserFavAdapter extends BaseUsersAdapter<UserFavAdapter<UserFav>> implements IUserFavAdapter {

    @Override
    protected String getServiceName() {
        return "userfav";
    }
    @Override
    protected UserFavAdapter<UserFav> makeObject() {
        return new UserFavAdapter<UserFav>(new WUserFavFactory());
    }
    @Override
    public void createUserFav(int id, UserFav userFav) {
        getAdapter().createUserFav(id, userFav);
    }

}
