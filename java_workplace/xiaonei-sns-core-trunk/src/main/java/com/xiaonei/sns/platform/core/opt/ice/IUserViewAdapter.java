/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserHomeView;

/**
 * a combin object of UserHomeView
 * 
 * @deprecated 由于中间层没有一个UserHomeView服务，所以这里直接去掉了。 (2010-02-05)
 * @author lookis (comic.liu@gmail.com)
 * @author xylz (xylz@live.cn)
 */
public interface IUserViewAdapter extends IRegisterable {

    /**
     * get a big object for {@link User}, {@link UserBasic},
     * {@link UserDesc}
     * 
     * @deprecated
     * @param userId the user id
     * @return {@link UserHomeView} Object or null while fetching null for
     *         any sub object
     */
    UserHomeView getUserHomeView(int userId);

}
