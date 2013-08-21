/**
 * $Id: IUserTimeAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserTime;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-2-5
 */
public interface IUserTimeAdapter extends IRegisterable {

    void createUserTime(int id, UserTime userTime);

    UserTime getUserTime(int id);

    void setUserTime(int id, UserTime userTime);
}
