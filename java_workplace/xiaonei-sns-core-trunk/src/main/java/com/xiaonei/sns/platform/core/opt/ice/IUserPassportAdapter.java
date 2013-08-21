/**
 * $Id: IUserPassportAdapter.java 15289 2010-01-19 07:28:09Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserPassport;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public interface IUserPassportAdapter extends IRegisterable {

    int getUserIdByEmail(String email);

    void createUserPassport(int userId, UserPassport userPassport);

    UserPassport getUserPassport(int userId);

    void setUserPassport(int userid, UserPassport userPassport);
}
