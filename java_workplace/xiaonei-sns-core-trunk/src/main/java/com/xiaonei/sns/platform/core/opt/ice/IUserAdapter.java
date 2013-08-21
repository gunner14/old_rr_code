/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.model.UserRight;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.model.UserUrl;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.userlogincount.UserLoginCountAdapter;
import com.xiaonei.xce.username.UserNameAdapter;
import com.xiaonei.xce.userpassport.UserPassportAdapter;
import com.xiaonei.xce.userright.UserRightAdapter;
import com.xiaonei.xce.userstate.UserStateAdapter;
import com.xiaonei.xce.userurl.UserUrlAdapter;

/**
 * User service
 * 
 * @author lookis (comic.liu@gmail.com)
 * @author xylz (xylz@live.cn)
 */
public interface IUserAdapter extends IRegisterable {

    User get(int id);

    /**
     * 根据email获取用户信息
     * 
     * @param email
     * @return 如果用户不存在返回null
     */
    User getByEmail(String email);

    /**
     * @deprecated This method will be remove after 20100219
     * @see SnsAdapterFactory#getUserLoginCountAdapter()
     */
    UserLoginCountAdapter getLoginCountAdapter();

    /**
     * @deprecated This method will be remove after 20100219
     * @see SnsAdapterFactory#getUserPassportAdapter()
     */
    UserPassportAdapter<UserPassport> getPassportAdapter();

    /**
     * @deprecated This method will be remove after 20100219
     * @see SnsAdapterFactory#getUserRightAdapter()
     */
    UserRightAdapter<UserRight> getRightAdapter();

    /**
     * @deprecated This method will be remove after 20100219
     * @see SnsAdapterFactory#getUserNameAdapter()
     */
    UserNameAdapter<UserName> getNameAdapter();

    /**
     * @deprecated This method will be remove after 20100219
     * @see SnsAdapterFactory#getUserStateAdapter()
     */
    UserStateAdapter<UserState> getStateAdapter();

    /**
     * @deprecated This method will be remove after 20100219
     * @see SnsAdapterFactory#getUserUrlAdapter()
     */
    UserUrlAdapter<UserUrl> getUrlAdapter();

}
