package com.xiaonei.platform.core.opt.permission;

import com.xiaonei.platform.core.opt.permission.source.Status;

public interface Role {

    public boolean isSocietyUser(Status guester);

    public boolean isActived(Status owner);

    public boolean isActive(Status guester);

    public boolean isSuicide(Status guester);

    public boolean isBanned(Status guester);

    public boolean isConfirm(Status guester);

    public boolean isConfirmed(Status owner);

    public boolean isNormal(Status guester);

    public boolean isStar(Status guester);

    /**
     * 管理员(普通管理员)
     * 
     * @param guester
     * @return
     */
    public boolean isAdmin(Status guester);

    /**
     * 超级管理员
     * 
     * @param guester
     * @return
     */
    public boolean isSuperAdmin(Status guester);

    /**
     * 不走广告antispam拦截
     * 
     * @param guester
     * @return
     */
    public boolean isAuthIgnoresAdAntispam(Status guester);

    /**
     * 突破个人主页隐私
     * 
     * @param guester
     * @return
     */
    public boolean isAuthIgnoresProfilePrivacy(Status guester);

    /**
     * 突破头像相册隐私
     * 
     * @param guester
     * @return
     */
    public boolean isAuthIgnoresPhotoPrivacy(Status guester);

    /**
     * 不留脚印
     * 
     * @param guester
     * @return
     */
    public boolean isAuthNonFootpoint(Status guester);

    /**
     * 短时间发太多相同内容权限
     * 
     * @param guester
     * @return
     */
    public boolean isAuthReplicatedUGC(Status guester);

    /**
     * 个人主页加星权限
     * 
     * @param guester
     * @return
     */
    public boolean isAuthStarUser(Status guester);

    /**
     * UGC上限权限
     * 
     * @param guester
     * @return
     */
    public boolean isAuthUGCLimit(Status guester);

    /**
     * 个人主页悄悄话
     * 
     * @param guester
     * @return
     */
    public boolean isAuthUserPrivateMessages(Status guester);

    /**
     * 删除UGC
     * 
     * @param guester
     * @return
     */
    public boolean isAuthDeleteUGC(Status guester);

    /**
     * UGC隐私
     * 
     * @param guester
     * @return
     */
    public boolean isAuthUGCPrivacy(Status guester);

}
