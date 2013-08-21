/**
 * $Id: IUserRightAdapter.java 15289 2010-01-19 07:28:09Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserRight;

/**
 * operations for UserRight
 * 
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public interface IUserRightAdapter extends IRegisterable {

    void createUserRight(int userId, UserRight userRight);

    UserRight getUserRight(int userId);

    void setUserRight(int userId, UserRight userRight);
}
