/**
 * $Id: IUserLoginCountAdapter.java 15289 2010-01-19 07:28:09Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

/**
 * service for 'UserLoginCount'
 * 
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public interface IUserLoginCountAdapter extends IRegisterable{

    /**
     * get the login count for user
     * 
     * @param userId the user id
     * @return the count of login or -1 while there is an error.
     */
    int getUserLoginCount(int userId);

    /**
     * increment the login count
     * 
     * @param userid the user id
     */
    void incUserLoginCount(int userid);
}
