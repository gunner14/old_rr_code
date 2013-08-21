/**
 * 
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserConfigInfo;

/**
 * User Config Info
 * 
 * @author lookis (comic.liu@gmail.com)
 * @author xylz (xylz@live.cn)
 */
public interface IUserConfigInfoAdapter extends IRegisterable {

    UserConfigInfo getUserConfig(int userId);

    void setUserConfig(int userid, UserConfigInfo uc);
    /**
     * 什么使用应该使用create而不是使用{@link #setUserConfig(int, UserConfigInfo)}，问中间层吧
     * 中间层答：当仅改变某个字段时，使用{@link #setUserConfig(int, UserConfigInfo)}。当且仅当注册新用户时，使用createUserConfig方法。
     * 相对应的后台SQL是UPDATE和INSERT。
     */
    void createUserConfig(int userId, UserConfigInfo uc);
}
