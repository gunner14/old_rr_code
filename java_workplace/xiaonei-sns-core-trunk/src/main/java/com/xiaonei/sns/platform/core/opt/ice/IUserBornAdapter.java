/**
 * $Id: IUserBornAdapter.java 15433 2010-01-20 07:19:05Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;


/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-20
 */
public interface IUserBornAdapter extends IRegisterable {
    void createUserBorn(int userId, UserBorn userBorn);
    UserBorn getUserBorn(int userId);
    void setUserBorn(int userId, UserBorn userBorn);
}
