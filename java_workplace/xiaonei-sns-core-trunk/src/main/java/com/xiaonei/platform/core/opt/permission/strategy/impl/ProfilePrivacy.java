package com.xiaonei.platform.core.opt.permission.strategy.impl;

import java.io.Serializable;
import java.text.ParseException;

import org.json.JSONObject;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.adminPermission.AdminManager;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;

/**
 * profile 隐私
 */
public class ProfilePrivacy implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = -1119658623733026416L;

    public static void main(final String[] args) {
        final ProfilePrivacy pp = new ProfilePrivacy(0, "{e:-1,w:3}", 0);
        System.out.println(pp.getShowEducation());
    }

    private int account = RelationDef.uuSchoolmate;

    private int address = RelationDef.uuSelf;

    private int album = RelationDef.uuSchoolmate;

    private int basicInfo = RelationDef.uuSchoolmate;

    private int blog = RelationDef.uuSchoolmate;

    private int classs = RelationDef.uuSchoolmate;

    private int education = RelationDef.uuSchoolmate;

    private int email = RelationDef.uuSelf;

    private int fixedtel = RelationDef.uuFriend;

    private int friend = RelationDef.uuNoPath;

    private int gift = RelationDef.uuSchoolmate;

    private int gossip = RelationDef.uuSchoolmate;

    private int gossip_board = RelationDef.uuNoPath;

    private int link = RelationDef.uuFriend;

    private int love = RelationDef.uuSelf;

    private int lover = RelationDef.uuSelf;

    private int mobiletel = RelationDef.uuFriend;

    private int msn = RelationDef.uuFriend;

    private final int path;

    private int personal = RelationDef.uuSchoolmate;

    private int qq = RelationDef.uuFriend;

    private int remark = RelationDef.uuSelf;

    private int share = RelationDef.uuSchoolmate;

    private int status = RelationDef.uuSchoolmate;

    private int tribe = RelationDef.uuSchoolmate;

    /** 访问者(访问主页的游客，当前登录者)的Auth值 */
    private final int vistorAuth;

    private int work = RelationDef.uuSchoolmate;

    int visitor = 0;

    /**
     * 缺省构造函数，<br>
     * 当用户没有设置过新的隐私信息时(profile_privacy为空)使用此构造函数
     * 
     * @param path
     */
    public ProfilePrivacy(final int path, final int vistorAuth) {
        this.path = path;
        this.vistorAuth = vistorAuth;
    }

    /**
     * 当用户设置过了新版隐私信息（profile_privacy有内容），使用此构造函数<br>
     * "{a:1,b:1,c:3,e:3,f:3,g:3,l:3,m:3,p:3,q:3,s:3,t:3,w:3,h:3,i:3}"
     * 
     * @param path
     * @param json json格式的串
     */
    public ProfilePrivacy(final int path, final String jsonstring, final int vistorAuth) {
        this.path = path;
        this.vistorAuth = vistorAuth;
        try {
            final JSONObject json = new JSONObject(jsonstring);
            if (json != null) {
                this.account = PrivacyProfileUtil.getValueFromJson(json, "a", this.account);
                this.basicInfo = PrivacyProfileUtil.getValueFromJson(json, "b", this.basicInfo);
                this.classs = PrivacyProfileUtil.getValueFromJson(json, "c", this.classs);
                this.education = PrivacyProfileUtil.getValueFromJson(json, "e", this.education);
                this.fixedtel = PrivacyProfileUtil.getValueFromJson(json, "i", this.fixedtel);
                this.friend = PrivacyProfileUtil.getValueFromJson(json, "f", this.friend);
                this.gossip = PrivacyProfileUtil.getValueFromJson(json, "g", this.gossip);
                this.gossip_board = PrivacyProfileUtil.getValueFromJson(json, "z",
                        this.gossip_board);
                this.blog = PrivacyProfileUtil.getValueFromJson(json, "j", this.blog);// 2
                this.album = PrivacyProfileUtil.getValueFromJson(json, "k", this.album);// 3
                this.link = PrivacyProfileUtil.getValueFromJson(json, "l", this.link);
                this.mobiletel = PrivacyProfileUtil.getValueFromJson(json, "h", this.mobiletel);
                this.msn = PrivacyProfileUtil.getValueFromJson(json, "m", this.msn);
                this.personal = PrivacyProfileUtil.getValueFromJson(json, "p", this.personal);
                this.qq = PrivacyProfileUtil.getValueFromJson(json, "q", this.qq);
                this.status = PrivacyProfileUtil.getValueFromJson(json, "s", this.status);
                this.tribe = PrivacyProfileUtil.getValueFromJson(json, "t", this.tribe);// 4
                this.work = PrivacyProfileUtil.getValueFromJson(json, "w", this.work);//
                this.share = PrivacyProfileUtil.getValueFromJson(json, "n", this.share);// 6
                this.gift = PrivacyProfileUtil.getValueFromJson(json, "d", this.gift);// 8
                this.email = PrivacyProfileUtil.getValueFromJson(json, "o", this.email);// 2010-8-1 email隐私
                this.address = PrivacyProfileUtil.getValueFromJson(json, "r", this.address);// 2010-8-1 地址隐私
                this.remark = PrivacyProfileUtil.getValueFromJson(json, "u", this.remark);// 2010-8-1 备注隐私
                this.love = PrivacyProfileUtil.getValueFromJson(json, "x1", this.love);
                this.lover = PrivacyProfileUtil.getValueFromJson(json, "y", this.lover);

            }
        } catch (final ParseException e) {
            System.err.println("error jsonstring->" + jsonstring);
            e.printStackTrace();
        }

    }

    public int getAccount() {
        return this.account;
    }

    /**
     * getAddress<br>
     * 2010-8-1 地址隐私
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 1, 2010 - 10:47:31 PM
     */
    public int getAddress() {
        return this.address;
    }

    public int getAlbum() {
        return this.album;
    }

    public int getBasicInfo() {
        return this.basicInfo;
    }

    public int getBlog() {
        return this.blog;
    }

    public int getClasss() {
        return this.classs;
    }

    public int getEducation() {
        return this.education;
    }

    /**
     * getEmail<br>
     * 2010-8-1 email 隐私
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 1, 2010 - 10:47:05 PM
     */
    public int getEmail() {
        return this.email;
    }

    public int getFixedtel() {
        return this.fixedtel;
    }

    public int getFriend() {
        return this.friend;
    }

    public int getGift() {
        return this.gift;
    }

    public int getGossip() {
        return this.gossip;
    }

    public int getGossip_board() {
        return this.gossip_board;
    }

    public int getLink() {
        return this.link;
    }

    public int getLove() {
        return this.love;
    }

    public int getLover() {
        return this.lover;
    }

    public int getMobiletel() {
        return this.mobiletel;
    }

    public int getMsn() {
        return this.msn;
    }

    public int getPath() {
        return this.path;
    }

    public int getPersonal() {
        return this.personal;
    }

    public int getQq() {
        return this.qq;
    }

    /**
     * getRemark<br>
     * 2010-8-1 备注隐私
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 1, 2010 - 10:47:51 PM
     */
    public int getRemark() {
        return this.remark;
    }

    /**
     * @param appId
     * @return
     */
    public int getRequiredBoxPrivacy(final int appId) {
        int privacy = 99;
        switch (appId) {
            case 100:// 特别好友
                privacy = this.getFriend();
                break;
            case 101:// 最近登录好友
                privacy = this.getFriend();
                break;
                // case 104://Mini-Feed
                // privacy = AppUser.BOX_PRIVACY_NO_ONE;
                // break;
            case 21:// 留言板
                privacy = this.getGossip();
                break;
            case 102:// 好友分布
                privacy = this.getFriend();
                break;
            case 103:// 工作教育信息
                privacy = this.getEducation();
                break;
        }
        return privacy;
    }

    public int getShare() {
        return this.share;
    }

    public boolean getShowAccount() {
        return (this.getShow(this.account) || ((this.visitor != 0) && AdminManager.getInstance()
                .hasRight(this.visitor, 162, 1)));
    }

    /**
     * getShowAddress<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 1, 2010 - 10:50:38 PM
     */
    public boolean getShowAddress() {
        return this.getShow(this.address);
    }

    public boolean getShowAlbum() {
        return this.getShow(this.album);
    }

    /**
     * 判断一个app的隐私设置，是否可见<br>
     * 李福松 2008-04-24 加此方法，给未来新版app隐私用
     * 
     * @param appPrivacy 某一个app的隐私设置值
     * @return
     */
    public boolean getShowApp(final int appPrivacy) {
        return this.getShow(appPrivacy);
    }

    public boolean getShowBasicInfo() {
        return (this.getShow(this.basicInfo) || ((this.visitor != 0) && AdminManager.getInstance()
                .hasRight(this.visitor, 162, 1)));
    }

    public boolean getShowBlog() {
        return this.getShow(this.blog);
    }

    public boolean getShowClass() {
        return this.getShow(this.classs);
    }

    public boolean getShowEducation() {
        return this.getShow(this.education);
    }

    /**
     * getShowEmail<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 1, 2010 - 10:50:32 PM
     */
    public boolean getShowEmail() {
        return this.getShow(this.email);
    }

    public boolean getShowFixedTel() {
        return this.getShow(this.fixedtel);
    }

    public boolean getShowFriend() {
        return this.getShow(this.friend);
    }

    public boolean getShowGift() {
        return this.getShow(this.gift);
    }

    /**
     * 几种允许浏览页面时的 甲-乙关系：<br>
     * 1、好友，不同网络 path=0<br>
     * 2、非好友，同网络 path=3<br>
     * 3、好友，同网络 path=0<br>
     * 
     * @return
     */
    public boolean getShowGossip() {
        if (this.gossip_board < this.gossip) {
            return this.getShowGossipBoard();
        }
        return (this.getShow(this.gossip) || ((this.visitor != 0) && AdminManager.getInstance()
                .hasRight(this.visitor, 162, 1)));
    }
    /**
     * 留言板是否可见
     * 
     * @return
     */
    public boolean getShowGossipBoard() {
        return this.getShow(this.gossip_board);
    }

    public boolean getShowLink() {
        return this.getShow(this.link);
    }

    public boolean getShowLove() {
        return this.getShow(this.love);

    }

    public boolean getShowLover() {
        return this.getShow(this.lover);

    }

    public boolean getShowMobileTel() {
        return this.getShow(this.mobiletel);
    }

    public boolean getShowMsn() {
        return this.getShow(this.msn);
    }

    public boolean getShowPersonal() {
        return this.getShow(this.personal);
    }

    public boolean getShowQQ() {
        return this.getShow(this.qq);
    }

    /**
     * getShowRemark<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 1, 2010 - 10:50:41 PM
     */
    public boolean getShowRemark() {
        return this.getShow(this.remark);
    }

    public boolean getShowShare() {
        return this.getShow(this.share);
    }

    public boolean getShowStatus() {
        return this.getShow(this.status);
    }

    public boolean getShowTribe() {
        return this.getShow(this.tribe);
    }

    public boolean getShowWork() {
        return this.getShow(this.work);
    }

    public int getStatus() {
        return this.status;
    }

    public int getTribe() {
        return this.tribe;
    }

    // public Status getVistor() {
    // return vistor;
    // }
    public int getWork() {
        return this.work;
    }

    public boolean isAdmin(final int vistorAuth) {
        return vistorAuth >= 5 ? true : false;
    }

    public boolean isSuperAdmin(final int vistorAuth) {
        return vistorAuth >= User.authSuperAdmin ? true : false;
    }

    public void setVisitor(final int visitor) {
        this.visitor = visitor;
    }

    /**
     * @param pp
     * @param thisShow
     * @return
     */
    private boolean getShow(final int pp) {
        if (this.isAdmin(this.vistorAuth) || this.isSuperAdmin(this.vistorAuth)) {// (超级)管理员
            return true;
        }
        return this.path <= pp ? true : false;
    }
}
