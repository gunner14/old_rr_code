/**
 * $Id: WUserStateAdapter.java 70383 2012-05-17 08:39:00Z chengkai.lin@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.model.factory.WUserStateFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserStateAdapter;
import com.xiaonei.xce.userstate.UserStateAdapter;

/**
 * @author xylz(xylz@live.cn)
 * @since 2010-1-19
 */
public class WUserStateAdapter extends BaseUsersAdapter<UserStateAdapter<UserState>> implements IUserStateAdapter {

    protected WUserStateAdapter() {
    }

    @Override
    public void createUserState(final int userId, final UserState userState) {
        this.getAdapter().createUserState(userId, userState);
    }

    @Override
    public UserState getUserState(final int userId) {
        return this.getAdapter().getUserState(userId);
    }

    @Override
    public void reload(final int userId) {
        //getAdapter().reload(userId);
    }

    @Override
    public void setCouplesSpaceOff(final int userId) {
        this.getAdapter().setCouplesSpaceOff(userId);
    }

    @Override
    public void setCouplesSpaceOn(final int userId) {
        this.getAdapter().setCouplesSpaceOn(userId);
    }

    @Override
    public void setEverVipMemberOff(final int userId) {
        this.getAdapter().setEverVipMemberOff(userId);
    }

    @Override
    public void setEverVipMemberOn(final int userId) {
        this.getAdapter().setEverVipMemberOn(userId);
    }

    @Override
    public void setGuideDoneOn(final int userId) {
        this.getAdapter().setGuideDoneOn(userId);
    }

    @Override
    public void setHideAdsOff(final int userId) {
        this.getAdapter().setHideAdsOff(userId);
    }

    @Override
    public void setHideAdsOn(final int userId) {
        this.getAdapter().setHideAdsOn(userId);
    }

    @Override
    public void setInWhiteListOff(final int userId) {
        this.getAdapter().setInWhiteListOff(userId);
    }

    @Override
    public void setInWhiteListOn(final int userId){
        this.getAdapter().setInWhiteListOn(userId);
    }

    @Override
    public void setKeepUseOff(final int userId) {
        this.getAdapter().setKeepUseOff(userId);
    }

    @Override
    public void setKeepUseOn(final int userId) {
        this.getAdapter().setKeepUseOn(userId);
    }

    @Override
    public void setLunarBirthOff(final int userId) {
        this.getAdapter().setLunarBirthOff(userId);
    }

    @Override
    public void setLunarBirthOn(final int userId) {
        this.getAdapter().setLunarBirthOn(userId);
    }

    @Override
    public void setMobileBindOff(final int userId) {
        this.getAdapter().setMobileBindOff(userId);
    }

    @Override
    public void setMobileBindOn(final int userId) {
        this.getAdapter().setMobileBindOn(userId);
    }

    @Override
    public void setMSNIMMEOff(final int userId) {
        this.getAdapter().setMSNIMMEOff(userId);
    }

    @Override
    public void setMSNIMMEOn(final int userId) {
        this.getAdapter().setMSNIMMEOn(userId);
    }

    @Override
    public void setPageManagerOff(final int userId) {
        this.getAdapter().setPageManagerOff(userId);
    }

    @Override
    public void setPageManagerOn(final int userId) {
        this.getAdapter().setPageManagerOn(userId);
    }

    @Override
    public void setSelectedOff(final int userId) {
        this.getAdapter().setSelectedOff(userId);
    }

    @Override
    public void setSelectedOn(final int userId) {
        this.getAdapter().setSelectedOn(userId);
    }

    @Override
    public void setSocialGraphGoodOff(final int userId) {
        this.getAdapter().setSocialGraphGoodOff(userId);
    }

    @Override
    public void setSocialGraphGoodOn(final int userId) {
        this.getAdapter().setSocialGraphGoodOn(userId);
    }

    @Override
    public void setUserState(final int userId, final UserState userState) {
        this.getAdapter().setUserState(userId, userState);
    }

    @Override
    public void setV6GuideOff(final int userId) {
        this.getAdapter().setV6GuideOff(userId);
    }

    @Override
    public void setV6GuideOn(final int userId) {
        this.getAdapter().setV6GuideOn(userId);
    }

    @Override
    public void setV6Off(final int userId) {
        this.getAdapter().setV6Off(userId);
    }

    @Override
    public void setV6On(final int userId) {
        this.getAdapter().setV6On(userId);
    }
    //inshion 2011-04-18 10:41:04 添加

    @Override
    public void setVipMemberOff(final int userId) {
        this.getAdapter().setVipMemberOff(userId);
    }

    @Override
    public void setVipMemberOn(final int userId) {
        this.getAdapter().setVipMemberOn(userId);
    }

    @Override
    public void setMpsMemberOn(final int userId) {
        this.getAdapter().setMpsMemberOn(userId);
    }

    @Override
    public void setMpsMemberOff(int userId) {
        this.getAdapter().setMpsMemberOff(userId);
    }
    
    @Override
    protected String getServiceName() {
        return "userstate";
    }

    @Override
    protected UserStateAdapter<UserState> makeObject() {
        return new UserStateAdapter<UserState>(new WUserStateFactory());
    }
}
