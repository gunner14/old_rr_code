/**
 * $Id: IUserStateAdapter.java 70383 2012-05-17 08:39:00Z chengkai.lin@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserState;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public interface IUserStateAdapter extends IRegisterable {

    void createUserState(int userId, UserState userState);

    UserState getUserState(int userId);

    void reload(int userId);

    void setCouplesSpaceOff(int userId);

    void setCouplesSpaceOn(int userId);

    void setEverVipMemberOff(int userId);

    void setEverVipMemberOn(int userId);

    void setGuideDoneOn(int userId);

    void setHideAdsOff(int userId);

    void setHideAdsOn(int userId);

    void setInWhiteListOff(int userId);

    //inshion 2011-08-04 12:15:24 添加
    void setInWhiteListOn(int userId);

    void setKeepUseOff(int userId);

    void setKeepUseOn(int userId);

    void setLunarBirthOff(int userId);

    //inshion 2011-04-18 10:41:04 添加
    void setLunarBirthOn(int userId);

    void setMobileBindOff(int userId);

    void setMobileBindOn(int userId);

    void setMSNIMMEOff(int userId);

    void setMSNIMMEOn(int userId);

    void setPageManagerOff(int userId);

    void setPageManagerOn(int userId);

    void setSelectedOff(int userId);

    void setSelectedOn(int userId);

    void setSocialGraphGoodOff(int userId);

    void setSocialGraphGoodOn(int userId);

    void setUserState(int userId, UserState userState);

    void setV6GuideOff(int userId);

    void setV6GuideOn(int userId);

    void setV6Off(int userId);

    void setV6On(int userId);

    void setVipMemberOff(int userId);

    void setVipMemberOn(int userId);
    
    void setMpsMemberOn(int userId);
    
    void setMpsMemberOff(int userId);
}
