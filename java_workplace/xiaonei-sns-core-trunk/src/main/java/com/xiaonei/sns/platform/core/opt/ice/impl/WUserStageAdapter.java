/**
 * $Id: WUserStageAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.sns.platform.core.opt.ice.IUserStageAdapter;
import com.xiaonei.sns.platform.core.opt.ice.model.UserStage;
import com.xiaonei.sns.platform.core.opt.ice.model.factory.WUserStageFactory;
import com.xiaonei.xce.userstage.UserStageAdapter;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-20
 */
public class WUserStageAdapter extends BaseUsersAdapter<UserStageAdapter<UserStage>> implements
        IUserStageAdapter {

    @Override
    protected UserStageAdapter<UserStage> makeObject() {
        return new UserStageAdapter<UserStage>(new WUserStageFactory());
    }

    @Override
    public void createUserStage(int userId, UserStage userStage) {
        checkUserId(userId);
        getAdapter().createUserStage(userId, userStage);
    }

    @Override
    public UserStage getUserStage(int userId) {
        checkUserId(userId);
        UserStage ret = null;
        try {
            ret = getAdapter().getUserStage(userId);
        } catch (Exception e) {
            rethrowBlockException(e, userId);
        }
        return ret;
    }

    @Override
    public void setUserStage(int userId, UserStage userStage) {
        checkUserId(userId);
        getAdapter().setUserStage(userId, userStage);
    }
    protected String getServiceName() {
        return "userstage";
    }
}
