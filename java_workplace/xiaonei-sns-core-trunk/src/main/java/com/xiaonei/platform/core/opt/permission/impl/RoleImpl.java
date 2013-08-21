package com.xiaonei.platform.core.opt.permission.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.permission.Role;
import com.xiaonei.platform.core.opt.permission.source.Status;

public class RoleImpl implements Role {

    private static RoleImpl instance = new RoleImpl();

    public static RoleImpl getInstance() {
        return instance;
    }

    public boolean isActived(Status guester) {
        if (OpiConstants.isKaixin()) { //开心不激活也认为它是激活的
            return guester.getStatus() <= User.statusActivateVerified ? true : false;
        }
        return guester.getStatus() < User.statusActivateVerified ? true : false;
    }

    public boolean isActive(Status guester) {
        if (OpiConstants.isKaixin()) {
            return false; //是false，没错
        }
        return guester.getStatus() == User.statusActivateVerified ? true : false;
    }

    /**
     * 管理员(普通管理员)?
     */
    public boolean isAdmin(Status guester) {
        return guester.getAuth() >= User.authAdmin;
    }

    /**
     * 超级管理员
     */
    public boolean isSuperAdmin(Status guester) {
        return guester.getAuth() >= User.authSuperAdmin;
    }

    /**
     * 删除UGC
     * 
     * @param guester
     * @return
     */
    public boolean isAuthDeleteUGC(Status guester) {
        return guester.getAuth() >= User.authDeleteUGC;
    }

    /**
     * 不走广告antispam拦截
     * 
     * @param guester
     * @return
     */
    public boolean isAuthIgnoresAdAntispam(Status guester) {
        return guester.getAuth() >= User.authIgnoresAdAntispam;
    }

    @Override
    public boolean isAuthIgnoresPhotoPrivacy(Status guester) {
        return guester.getAuth() >= User.authIgnoresPhotoPrivacy;
    }

    @Override
    public boolean isAuthIgnoresProfilePrivacy(Status guester) {
        return guester.getAuth() >= User.authIgnoresProfilePrivacy;
    }

    /**
     * 不留脚印
     * 
     * @param guester
     * @return
     */
    public boolean isAuthNonFootpoint(Status guester) {
        return guester.getAuth() >= User.authNonFootpoint;
    }

    /**
     * 短时间发太多相同内容权限
     * 
     * @param guester
     * @return
     */
    public boolean isAuthReplicatedUGC(Status guester) {
        return guester.getAuth() >= User.authReplicatedUGC;
    }

    /**
     * 个人主页加星权限
     * 
     * @param guester
     * @return
     */
    public boolean isAuthStarUser(Status guester) {
        return guester.getAuth() >= User.authStarUser;
    }

    /**
     * UGC上限权限
     * 
     * @param guester
     * @return
     */
    public boolean isAuthUGCLimit(Status guester) {
        return guester.getAuth() >= User.authUGCLimit;
    }

    /**
     * 个人主页悄悄话
     * 
     * @param guester
     * @return
     */
    public boolean isAuthUserPrivateMessages(Status guester) {
        return guester.getAuth() >= User.authUserPrivateMessages;
    }
    
    @Override
    public boolean isAuthUGCPrivacy(Status guester) {
        return guester.getAuth() >= User.authUGCPrivacy;
    }

    public boolean isSocietyUser(Status guester) {
        return guester.getStatus() <= User.statusActivateVerified ? true : false;
    }

    public boolean isSuicide(Status guester) {
        return guester.getStatus() == User.statusSuicide ? true : false;
    }

    public boolean isBanned(Status guester) {
        return guester.getStatus() == User.statusBanned ? true : false;

    }

    public boolean isConfirm(Status guester) {
        if (guester.getStatus() == User.statusEnsure) return true;
        return false;
    }

    public boolean isConfirmed(Status guester) {
        if (OpiConstants.isKaixin()) {
            return guester.getStatus() <= User.statusActivateVerified;
        } else {
            return guester.getStatus() < User.statusEnsure;
        }
    }

    public boolean isNormal(Status guester) {
        if (guester.getStatus() < User.statusEnsure) return true;
        return false;
    }

    public boolean isStar(Status guester) {
        return guester.getSelected() == User.usrSelected ? true : false;
    }

}
