/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserBasic;

/**
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public interface IUserBasicAdapter extends IRegisterable {

    UserBasic get(int id);
    /*
    void setUserBasic(int userid, UserBasic ubi);

    void createUserBasic(int userId, UserBasic ubi);
    */
}
