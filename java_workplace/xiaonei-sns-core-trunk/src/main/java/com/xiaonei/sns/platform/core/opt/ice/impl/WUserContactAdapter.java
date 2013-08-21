/**
 * $Id: WUserContactAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserContact;
import com.xiaonei.platform.core.model.factory.WUserContactFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserContactAdapter;
import com.xiaonei.xce.usercontact.UserContactAdapter;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-2-5
 */
public class WUserContactAdapter extends BaseUsersAdapter<UserContactAdapter<UserContact>> implements IUserContactAdapter {

    @Override
    protected String getServiceName() {
        return "usercontact";
    }
    @Override
    protected UserContactAdapter<UserContact> makeObject() {
        return new UserContactAdapter<UserContact>(new WUserContactFactory());
    }
    @Override
    public void createUserContact(int id, UserContact userContact) {
        getAdapter().createUserContact(id, userContact);

    }

    @Override
    public UserContact getUserContact(int id) {
        return getAdapter().getUserContact(id);
    }

    @Override
    public void setUserContact(int id, UserContact userContact) {
        getAdapter().setUserContact(id, userContact);

    }

}
