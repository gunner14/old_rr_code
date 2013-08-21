/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserContact;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserFav;
import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.xce.usercontact.UserContactAdapter;
import com.xiaonei.xce.userfav.UserFavAdapter;
import com.xiaonei.xce.usertime.UserTimeAdapter;

/**
 * UserDesc service
 * 
 * @author lookis (comic.liu@gmail.com)
 * @author xylz (xylz@live.cn)
 */
public interface IUserDescAdapter extends IRegisterable {

    UserDesc get(int id);

    /**
     * @deprecated 2010-02-05
     * @see IUserTimeAdapter
     */
    UserTimeAdapter<UserTime> getTimeAdapter();

    /**
     * @deprecated 2010-02-05
     * @see IUserContactAdapter
     */
    UserContactAdapter<UserContact> getContactAdapter();

    /**
     * @deprecated 2010-02-05
     * @see IUserFavAdapter
     */
    UserFavAdapter<UserFav> getFavAdapter();

}
