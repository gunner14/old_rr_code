/**
 * $Id: IUserUrlAdapter.java 15289 2010-01-19 07:28:09Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserUrl;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public interface IUserUrlAdapter extends IRegisterable {

    UserUrl getUserUrl(int id);

    void setUserUrl(int id, UserUrl userUrl);

    void createUserUrl(int id, UserUrl userUrl);

    void reload(int id);
}
