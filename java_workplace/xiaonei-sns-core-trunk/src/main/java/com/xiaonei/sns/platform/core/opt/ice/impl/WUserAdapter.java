package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.model.UserRight;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.model.UserUrl;
import com.xiaonei.platform.core.model.factory.WUserCommonFactory;
import com.xiaonei.platform.core.model.factory.WUserNameFactory;
import com.xiaonei.platform.core.model.factory.WUserPassportFactory;
import com.xiaonei.platform.core.model.factory.WUserRightFactory;
import com.xiaonei.platform.core.model.factory.WUserStateFactory;
import com.xiaonei.platform.core.model.factory.WUserUrlFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserAdapter;
import com.xiaonei.xce.userbaseview.UserBaseCommonViewAdapter;
import com.xiaonei.xce.userlogincount.UserLoginCountAdapter;
import com.xiaonei.xce.username.UserNameAdapter;
import com.xiaonei.xce.userpassport.UserPassportAdapter;
import com.xiaonei.xce.userright.UserRightAdapter;
import com.xiaonei.xce.userstate.UserStateAdapter;
import com.xiaonei.xce.userurl.UserUrlAdapter;

/** Adapter for 'User' Object
 * @author zhangjie@opi-corp.com
 * @author xylz (xylz@live.cn)
 */
public class WUserAdapter
        extends
        BaseUsersAdapter<UserBaseCommonViewAdapter<User, UserPassport, UserRight, UserName, UserState, UserUrl>>
        implements IUserAdapter {

    private UserLoginCountAdapter loginCountAdapter;

    private UserPassportAdapter<UserPassport> passportAdapter;

    private UserRightAdapter<UserRight> rightAdapter;

    private UserNameAdapter<UserName> nameAdapter;

    private UserStateAdapter<UserState> stateAdapter;

    private UserUrlAdapter<UserUrl> urlAdapter;

    protected WUserAdapter() {
        loginCountAdapter = new UserLoginCountAdapter();
        passportAdapter = new UserPassportAdapter<UserPassport>(new WUserPassportFactory());
        rightAdapter = new UserRightAdapter<UserRight>(new WUserRightFactory());
        nameAdapter = new UserNameAdapter<UserName>(new WUserNameFactory());
        stateAdapter = new UserStateAdapter<UserState>(new WUserStateFactory());
        urlAdapter = new UserUrlAdapter<UserUrl>(new WUserUrlFactory());
    }

    /**
     * @deprecated
     * @see {@linkplain SnsAdapterFactory#getUserAdapter()}
     */
    public static IUserAdapter getInstance() {
        return SnsAdapterFactory.getUserAdapter();
    }

    public User get(int id) {
        checkUserId(id);
        User u = null;
        try {
            u = getAdapter().getUserBaseView(id);
        } catch (Exception e) {
            System.err.println("Get User Error:" + id);
            rethrowBlockException(e, id);
        }
        return u;
    }

    public User getByEmail(String email) {
        int userId = SnsAdapterFactory.getUserPassportAdapter().getUserIdByEmail(email);
        return get(userId);
    }

    public UserLoginCountAdapter getLoginCountAdapter() {
        return loginCountAdapter;
    }

    public UserPassportAdapter<UserPassport> getPassportAdapter() {
        return passportAdapter;
    }

    public UserRightAdapter<UserRight> getRightAdapter() {
        return rightAdapter;
    }

    public UserNameAdapter<UserName> getNameAdapter() {
        return nameAdapter;
    }

    public UserStateAdapter<UserState> getStateAdapter() {
        return stateAdapter;
    }

    public UserUrlAdapter<UserUrl> getUrlAdapter() {
        return urlAdapter;
    }

    @Override
    protected UserBaseCommonViewAdapter<User, UserPassport, UserRight, UserName, UserState, UserUrl> makeObject() {
        return new UserBaseCommonViewAdapter<User, UserPassport, UserRight, UserName, UserState, UserUrl>(
                new WUserCommonFactory(), new WUserPassportFactory(), new WUserRightFactory(),
                new WUserNameFactory(), new WUserStateFactory(), new WUserUrlFactory());
    }
    @Override
    protected String getServiceName() {
        return "user";
    }
}
