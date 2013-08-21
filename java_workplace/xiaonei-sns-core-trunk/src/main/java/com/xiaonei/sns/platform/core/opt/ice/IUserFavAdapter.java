/**
 * $Id: IUserFavAdapter.java 16544 2010-02-05 11:23:14Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserFav;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-2-5
 */
public interface IUserFavAdapter extends IRegisterable {

    void createUserFav(int id, UserFav userFav);
}
