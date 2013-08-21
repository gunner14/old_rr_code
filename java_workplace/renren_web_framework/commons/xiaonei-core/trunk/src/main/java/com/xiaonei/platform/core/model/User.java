package com.xiaonei.platform.core.model;

import xce.userbase.CStateDOUBLEACCOUNT;
import xce.userbase.CStateMOBILEBIND;
import xce.userbase.CStateMSNIMME;
import xce.userbase.CStateSELECTED;
import xce.userbase.CStateVIPMEMBER;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.xce.userbaseview.UserBaseCommonView;

/**
 * User.java
 * 
 * @author zhangjie@opi-corp.com
 */
public class User extends UserBaseCommonView<UserPassport, UserRight, UserName, UserState, UserUrl> {

    public static final int usrNotSelected = 0;

    public static final int usrSelected = 1;

    public static final int statusAudit = 8;

    public static final int statusSuicide = 7;

    public static final int statusBanned = 6;

    public static final int statusActivateVerify = 5;

    public static final int statusActivatedVerify = 4;

    public static final int statusActivateVerified = 3;

    public static final int statusEnsure = 2;

    public static final int statusOnLine = 1;

    public static final int statusNormal = 0;

    /**
     * 糯米等子站导入的用户已经激活，尚未转为人人用户
     * 
     * 2011-05-27，人人糯米合并
     */
    public static final int statusImportedUserActivated = 10;
    
    /**
     * 糯米等子站导入的用户未激活，尚未转为人人用户
     * 
     * 2011-05-27，人人糯米合并
     */
    public static final int statusImportedUserNotActivated = 13;
    
    public static final int RIGHT_DESC = 0x00000040;

    public static final int RIGHT_COMMENT = 0x00000020;// blog模块用到的

    public static final int RIGHT_LEAVEWORD = 0x00000010;

    public static final int RIGHT_ANN = 0x00000008;

    public static final int RIGHT_EVENT = 0x00000004;

    public static final int RIGHT_MAIL = 0x00000002;

    public static final int RIGHT_POST = 0x00000001;

    public static final int defaultUserRights = RIGHT_MAIL | RIGHT_POST | RIGHT_ANN | RIGHT_EVENT
            | RIGHT_LEAVEWORD | RIGHT_COMMENT;

    /**
     * 普通管理员
     */
    public static final int authAdmin = 5;

    public static final int authOwner = 10;

    /**
     * 超级管理员
     */
    public static final int authSuperAdmin = 20;

    /**
     * 突破隐私
     */
    public static final int authIgnoresProfilePrivacy = 10;

    /**
     * 突破隐私
     */
    public static final int authIgnoresPhotoPrivacy = 10;

    /**
     * 不留脚印
     */
    public static final int authNonFootpoint = 10;

    /**
     * 删除UGC
     */
    public static final int authDeleteUGC = 15;

    /**
     * 个人主页悄悄话
     */
    public static final int authUserPrivateMessages = 15;

    /**
     * 个人主页加星权限
     */
    public static final int authStarUser = 10;

    /**
     * 短时间发太多相同内容权限
     */
    public static final int authReplicatedUGC = 5;

    /**
     * 不走广告antispam拦截
     */
    public static final int authIgnoresAdAntispam = 5;

    /**
     * UGC上限
     */
    public static final int authUGCLimit = 5;

    /**
     * UGC隐私
     */
    public static final int authUGCPrivacy = 15;

    public static final String genderUnknown = "";

    public static final String genderMale = "男生";

    public static final String genderFemale = "女生";

    public static final int userId_Max = 100000000;

    public static final int USER_SOURCE_DEFAULT = 0;

    public static final int USER_SOURCE_5Q = 5;

    private static final long serialVersionUID = 4825282963196348511L;

    /** pageId起始值 */
    private static final int PAGE_ID_START = 600000000;

    /** pageId结束值 */
    private static final int PAGE_ID_END = 700000000;

    /** app pageId起始值 */
    private static final int PAGE_ID_FOR_APP_START = 699000000;

    /** app pageId结束值 */
    private static final int PAGE_ID_FOR_APP_END = PAGE_ID_END;

    /**
     * 是公共主页用户(包括app的)?
     * 
     * @param id
     * @return
     */
    public static boolean isPageId(int id) {
        return (id >= PAGE_ID_START) && (id < PAGE_ID_END);
    }

    /**
     * 是app的id?
     * 
     * @param id
     * @return
     */
    public static boolean isAppPageId(int id) {
        return id > PAGE_ID_FOR_APP_START && id < PAGE_ID_FOR_APP_END;
    }

    public User() {
    }

    public UserState getUserStateInfo() {
        return this.userState;
    }

    public int getId() {
        return this.userName.getId();
    }

    // Old User Remain Begin
    /**
     * 获取用户登录帐号，为兼容老接口，不再推荐使用
     * 
     * @deprecated
     */
    public String getEmail() {
        return this.userPassport.getAccount();
    }

    // Old User Remain End

    // UserName Begin
    /**
     * 获取用户姓名
     */
    public String getName() {
        return this.userName.getName();
    }

    /**
     * 获取用户昵称
     * 
     * @return
     */
    public String getNickname() {
        return this.userName.getNickname();
    }

    // UserName End

    // UserPassport Begin
    /**
     * 获取用户登录帐号，代替原来的email
     */
    public String getAccount() {
        return this.userPassport.getAccount();
    }

    /**
     * 获取用户注册时所在域名
     * 
     * @return
     */
    public String getDomain() {
        return this.userPassport.getDomain();
    }

    /**
     * 获取md5后的密码密文
     * 
     * @return
     */
    public String getPassword_md5() {
        return this.userPassport.getPasswordMd5();
    }

    /**
     * 获取用户帐号当前状态 0:激活；3:未激活；6:封禁；7:自杀
     * 
     * @return
     */
    public int getStatus() {
        return this.userPassport.getStatus();
    }

    public int getSafeStatus() {
        return this.userPassport.getSafeStatus();
    }

    // UserPassport End

    // UserRight Begin
    public int getAuth() {
        return this.userRight.getAuth();
    }

    public int getRights() {
        return this.userRight.getRights();
    }

    private boolean hasCommonRight(int rightName) {
        return (this.userRight.getRights() & rightName) > 0;
    }

    public void setCommonRightOn(int rightName) {
        this.userRight.setRights(this.userRight.getRights() | rightName);
    }

    public void setCommonRightOff(int rightName) {
        this.userRight.setRights(this.userRight.getRights() & (~rightName));
    }

    public boolean getEventRight() {
        return hasCommonRight(User.RIGHT_EVENT);
    }

    public boolean getMailRight() {
        return hasCommonRight(User.RIGHT_MAIL);
    }

    public boolean getAnnounceRight() {
        return hasCommonRight(User.RIGHT_ANN);
    }

    public boolean getPostRight() {
        return hasCommonRight(User.RIGHT_POST);
    }

    public boolean getLeaveWordRight() {
        return hasCommonRight(User.RIGHT_LEAVEWORD);
    }

    public boolean getCommentRight() {
        return hasCommonRight(User.RIGHT_COMMENT);
    }

    // UserRight End

    // UserState Begin
    /**
     * 标识用户身份，不同的bit标识不同的身份，单独获取没有意义
     */
    public int getState() {
        return userState.getState();
    }

    /**
     * 判断是不是星级用户，兼容老接口，不再推荐使用
     * 
     * @deprecated
     * @return 1为星级用户，0为非星级用户
     */
    public int getSelected() {
        if (isStarUser()) return 1;
        return 0;
    }

    /**
     * 是否为星级用户
     * 
     * @return
     */
    public boolean isStarUser() {
        return 0 != (userState.getState() & (1 << CStateSELECTED.value));
    }

    /**
     * 是否为vip用户
     * 
     * @return
     */
    public boolean isVipMember() {
        return 0 != (this.userState.getState() & (1 << CStateVIPMEMBER.value));
    }

    /**
     * 是否为msn绑定用户
     * 
     * @return
     */
    public boolean isMSNIMME() {
        return 0 != (this.userState.getState() & (1 << CStateMSNIMME.value));
    }

    /**
     * 是否为手机绑定用户
     * 
     * @return
     */
    public boolean isMobileBind() {
        return 0 != (this.userState.getState() & (1 << CStateMOBILEBIND.value));
    }

    /**
     * 是否是双帐号用户
     * 
     * @return
     */
    public boolean isDoubleAccount() {
    	return 0 != (this.userState.getState() & (1 << CStateDOUBLEACCOUNT.value));
    }
    
	/**
	 * @return 是否为子站（如糯米）导入用户，且尚未转换给人人用户
	 */
	public boolean isImportedUser() {
		return getStatus() == statusImportedUserActivated
				|| getStatus() == statusImportedUserNotActivated;
	}
    
    /**
     * 星级级别：1为头像真实，2为姓名真实，3为两者都是
     * 
     * @return
     */
    public int getStar() {
        return this.userState.getStar();
    }

    /**
     * 尚未使用
     * 
     * @return
     */
    public int getLevel() {
        return this.userState.getLevel();
    }

    // UserState End

    // UserLogin Begin
    /**
     * 用户登录次数
     */
    public int getLoginCount() {
        return this.userLoginCount;
    }

    // UserLogin End

    //UserUrl begin
    /**
     * 用户头像(相对地址)
     */
    public String getHeadUrl() {
        return userUrl.getHeadUrl();
    }

    /**
     * 用户大头像(相对地址)
     * 
     * @return
     */
    public String getLargeUrl() {
        return userUrl.getLargeUrl();
    }

    /**
     * 也是用户头像
     * 
     * @return
     */
    public String getMainUrl() {
        return userUrl.getMainUrl();
    }

    /**
     * 用户小头像
     * 
     * @return
     */
    public String getTinyUrl() {
        return userUrl.getTinyUrl();
    }

    /**
     * 用户头像(绝对路径)
     * 
     * @return
     */
    public String getHeadFullUrl() {
        return HeadUrlUtil.getHeadFullUrl(getHeadUrl());
    }

    public String getTinyFullUrl() {
        return HeadUrlUtil.getHeadFullUrl(getTinyUrl());
    }

    public String getLargeFullUrl() {
        return HeadUrlUtil.getHeadFullUrl(getLargeUrl());
    }

    public String getMainFullUrl() {
        return HeadUrlUtil.getHeadFullUrl(getMainUrl());
    }
    
    /**
     * 判断当前User对象是不是一个page，如果是page的话，
     * 则还会有一个realUserId字段，并且realUserId!=id
     * 
     * @return 当前User是不是一个page
     */
    public boolean isPage() {
    	return this.getRealUserId() != 0 && this.getId() != this.getRealUserId();
    }
    //UserUrl end
}
