package com.xiaonei.platform.core.opt.permission.strategy.impl;

import java.text.ParseException;

import org.apache.struts.action.ActionForm;
import org.apache.struts.validator.DynaValidatorForm;
import org.json.JSONObject;

import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 
 * 隐私设置中的个项目的key-value对照表
 */
public class PrivacyProfileUtil {

    /*
     * a-account
     * b-basicInfo
     * c-class
     * d-gift
     * e-education
     * f-friend
     * g-gossip
     * h-mobiletel
     * i-fixedtel
     * j-blog
     * k-album
     * l-link
     * m-msn
     * n-share
     * o-email //2010-8-1 email隐私
     * p-personal
     * q-qq
     * r-address //2010-8-1 地址隐私
     * s-status
     * t-tribe
     * u-remark //2010-8-1 备注隐私
     * v-follow
     * w-work
     * x -
     * x1- love //用户当前感情状态的隐私
     * y- lover //用户感情状态中涉及到的人
     * z-gossip board
     */

    /** 隐私设置每一单项的缺省值 */

    private static String p_account = "p_account";

    private static String p_album = "p_album";

    private static String p_blog = "p_blog";

    // private static String p_basic = "p_basic";
    
    private static String p_class = "p_class";

    private static String p_education = "p_education";

    private static String p_fixedtel = "p_fixedtel";

    private static String p_follow = "p_follow";// 2009-05-25,加入了follow的隐私设置

    private static String p_friend = "p_friend";

    private static String p_gift = "p_gift";

    private static String p_gossip = "p_gossip";

    private static String p_gossip_board = "p_gossip_board";

    private static String p_link = "p_link";

    //private static String p_love = "p_love";

    //private static String p_lover = "p_lover";

    private static String p_mobiletel = "p_mobiletel";

    private static String p_msn = "p_msn";

    private static String p_personal = "p_personal";

    private static String p_qq = "p_qq";

    private static String p_share = "p_share";

    private static String p_status = "p_status";

    // private static String p_email = "p_email"; //2010-8-1 email隐私
    //
    // private static String p_address = "p_address"; //2010-8-1 地址隐私
    //
    // private static String p_remark = "p_remark"; //2010-8-1 备注隐私

    private static String p_tribe = "p_tribe";

    private static String p_work = "p_work";

    /**
     * 获得默认的隐私设置的JSON窜，全站统一用这个方法生成默认的窜，此方法在开心上使用p_follow，校内上未验证是否可用
     * p_follow
     * @author zhongming.jiang
     * @return　String
     */
    public static String ajaxJsonString() {
        final StringBuilder sb = new StringBuilder("{");
        sb.append(PrivacyProfileUtil.getstr1('a', RelationDef.uuNoPath));
        sb.append(PrivacyProfileUtil.getstr1('c', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('e', RelationDef.uuNoPath));
        sb.append(PrivacyProfileUtil.getstr1('f', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('g', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('z', RelationDef.uuNoPath));
        sb.append(PrivacyProfileUtil.getstr1('j', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('k', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('l', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('m', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('p', RelationDef.uuNoPath));
        sb.append(PrivacyProfileUtil.getstr1('q', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('s', RelationDef.uuNoPath));
        sb.append(PrivacyProfileUtil.getstr1('t', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('w', RelationDef.uuNoPath));
        sb.append(PrivacyProfileUtil.getstr1('h', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('n', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('d', RelationDef.uuFriend));
        sb.append(PrivacyProfileUtil.getstr1('v', RelationDef.uuNoPath));// 2009-05-25,加入了follow的隐私设置
        // sb.append(getstr1('o', RelationDef.uuSelf)); //2010-8-1 email隐私
        // sb.append(getstr1('r', RelationDef.uuSelf)); //2010-8-1 地址隐私
        // sb.append(getstr1('u', RelationDef.uuSelf)); //2010-8-1 备注隐私
        sb.append(PrivacyProfileUtil.getstr1('x', RelationDef.uuSelf)); // 2010-8-2 名片隐私
        sb.append(PrivacyProfileUtil.getstr1("x1", RelationDef.uuSelf));
        sb.append(PrivacyProfileUtil.getstr1('y', RelationDef.uuSelf));
        sb.append('i' + ":" + RelationDef.uuFriend);
        sb.append("}");
        return sb.toString();
    }

    /**
     * 收集页面元素拼成JSON大串
     * 
     * @param form
     * @return
     */
    public static String form2JsonString(final ActionForm form) {
        String str = "{";
        final DynaValidatorForm editForm = (DynaValidatorForm) form;
        str += PrivacyProfileUtil.getstr1('a', editForm.get(PrivacyProfileUtil.p_account));
        str += PrivacyProfileUtil.getstr1('c', editForm.get(PrivacyProfileUtil.p_class));
        str += PrivacyProfileUtil.getstr1('e', editForm.get(PrivacyProfileUtil.p_education));
        str += PrivacyProfileUtil.getstr1('f', editForm.get(PrivacyProfileUtil.p_friend));
        str += PrivacyProfileUtil.getstr1('g', editForm.get(PrivacyProfileUtil.p_gossip));
        str += PrivacyProfileUtil.getstr1('z', editForm.get(PrivacyProfileUtil.p_gossip_board));
        str += PrivacyProfileUtil.getstr1('j', editForm.get(PrivacyProfileUtil.p_blog));
        str += PrivacyProfileUtil.getstr1('k', editForm.get(PrivacyProfileUtil.p_album));
        str += PrivacyProfileUtil.getstr1('l', editForm.get(PrivacyProfileUtil.p_link));
        str += PrivacyProfileUtil.getstr1('m', editForm.get(PrivacyProfileUtil.p_msn));
        str += PrivacyProfileUtil.getstr1('p', editForm.get(PrivacyProfileUtil.p_personal));
        str += PrivacyProfileUtil.getstr1('q', editForm.get(PrivacyProfileUtil.p_qq));
        str += PrivacyProfileUtil.getstr1('s', editForm.get(PrivacyProfileUtil.p_status));
        str += PrivacyProfileUtil.getstr1('t', editForm.get(PrivacyProfileUtil.p_tribe));
        str += PrivacyProfileUtil.getstr1('w', editForm.get(PrivacyProfileUtil.p_work));
        str += PrivacyProfileUtil.getstr1('h', editForm.get(PrivacyProfileUtil.p_mobiletel));
        str += PrivacyProfileUtil.getstr1('n', editForm.get(PrivacyProfileUtil.p_share));
        str += PrivacyProfileUtil.getstr1('d', editForm.get(PrivacyProfileUtil.p_gift));
        str += PrivacyProfileUtil.getstr1('v', editForm.get(PrivacyProfileUtil.p_follow)); // 2009-05-25,加入了follow的隐私设置
        // str += getstr1('o', editForm.get(p_email)); //2010-8-1 email隐私
        // str += getstr1('r', editForm.get(p_address)); //2010-8-1 地址隐私
        // str += getstr1('u', editForm.get(p_remark)); //2010-8-1 备注隐私
        //str += PrivacyProfileUtil.getstr1("x1", editForm.get(PrivacyProfileUtil.p_love));
        //str += PrivacyProfileUtil.getstr1('y', editForm.get(PrivacyProfileUtil.p_lover));
        str += ('i' + ":" + editForm.get(PrivacyProfileUtil.p_fixedtel));
        str += "}";
        return str;
    }

    public static int getLovePrivacy(final UserConfigInfo uc) {
        final JSONObject jsonObject = PrivacyProfileUtil.getPpJSONObject(uc.getProfilePrivacy());
        return jsonObject.has("x1") ? jsonObject.getInt("x1") : RelationDef.uuSelf;
    }

    public static int getLoverPrivacy(final UserConfigInfo uc) {
        final JSONObject jsonObject = PrivacyProfileUtil.getPpJSONObject(uc.getProfilePrivacy());
        return jsonObject.has("y") ? jsonObject.getInt("y") : RelationDef.uuSelf;
    }

    /**
     * 得到JSON对象
     * 
     * @param ppString
     * @return
     */
    public static JSONObject getPpJSONObject(final String ppString) {
        if ((ppString == null) || (ppString.trim().length() == 0)) {
            return null;
        }
        JSONObject jsonObject = null;
        try {
            jsonObject = new JSONObject(ppString);
        } catch (final ParseException e) {
            e.printStackTrace(System.err);
        }
        return jsonObject;
    }

    public static int getValueFromJson(final JSONObject json, final String c, final int oldValue) {
        try {
            return json.getInt(c);
        } catch (final Exception e) {
            // e.printStackTrace(System.err);
        }
        return oldValue;
    }

    /**
     * 根据JSON串组装Form对象
     * 
     * @param editForm
     * @param ppString
     * @return
     */
    public static DynaValidatorForm jsonString2form(final DynaValidatorForm editForm,
            final String ppString) {
        if (ppString == null) {
            if (com.xiaonei.platform.core.opt.OpiConstants.isXiaonei()) {
                editForm.set(PrivacyProfileUtil.p_account, RelationDef.uuSchoolmate);// PrivacyItem_MyFriends_and_MyNetworks
                // 3
                editForm.set(PrivacyProfileUtil.p_friend, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_personal, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_status, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_link, RelationDef.uuFriend);// PrivacyItem_OnlyMyFriends
                // 1
                editForm.set(PrivacyProfileUtil.p_msn, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_qq, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_mobiletel, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_fixedtel, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_class, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_education, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_gossip, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_gossip_board, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_tribe, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_work, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_blog, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_album, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_share, RelationDef.uuSchoolmate);
                editForm.set(PrivacyProfileUtil.p_gift, RelationDef.uuSchoolmate);
                //editForm.set(PrivacyProfileUtil.p_love, RelationDef.uuSelf);
                //editForm.set(PrivacyProfileUtil.p_lover, RelationDef.uuSelf);
                // editForm.set(p_email, RelationDef.uuSelf);//2010-8-1 email隐私
                // editForm.set(p_address, RelationDef.uuSelf);//2010-8-1 地址隐私
                // editForm.set(p_remark, RelationDef.uuSelf);//2010-8-1 备注隐私
            } else if (com.xiaonei.platform.core.opt.OpiConstants.isKaixin()) {
                editForm.set(PrivacyProfileUtil.p_account, RelationDef.uuNoPath);// PrivacyItem_MyFriends_and_MyNetworks
                // 3
                editForm.set(PrivacyProfileUtil.p_friend, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_personal, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_status, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_link, RelationDef.uuFriend);// PrivacyItem_OnlyMyFriends
                // 1
                editForm.set(PrivacyProfileUtil.p_msn, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_qq, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_mobiletel, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_fixedtel, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_class, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_education, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_gossip, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_gossip_board, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_tribe, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_work, RelationDef.uuNoPath);
                editForm.set(PrivacyProfileUtil.p_blog, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_album, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_share, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_gift, RelationDef.uuFriend);
                editForm.set(PrivacyProfileUtil.p_follow, RelationDef.uuNoPath);// 2009-05-25,加入了follow的隐私设置
            } else {
                System.out.println("----------domainCode is error");
            }
        } else {
            final JSONObject json = PrivacyProfileUtil.getPpJSONObject(ppString);
            if (json != null) {
                if (com.xiaonei.platform.core.opt.OpiConstants.isXiaonei()) {
                    editForm.set(PrivacyProfileUtil.p_account, PrivacyProfileUtil.getValueFromJson(json, "a", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_friend, PrivacyProfileUtil.getValueFromJson(json, "f", RelationDef.uuNoPath));
                    editForm.set(PrivacyProfileUtil.p_personal, PrivacyProfileUtil.getValueFromJson(json, "p", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_status, PrivacyProfileUtil.getValueFromJson(json, "s", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_link, PrivacyProfileUtil.getValueFromJson(json, "l", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_msn, PrivacyProfileUtil.getValueFromJson(json, "m", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_qq, PrivacyProfileUtil.getValueFromJson(json, "q", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_mobiletel, PrivacyProfileUtil.getValueFromJson(json, "h", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_fixedtel, PrivacyProfileUtil.getValueFromJson(json, "i", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_class, PrivacyProfileUtil.getValueFromJson(json, "c", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_education, PrivacyProfileUtil.getValueFromJson(json, "e", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_gossip, PrivacyProfileUtil.getValueFromJson(json, "g", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_gossip_board, PrivacyProfileUtil.getValueFromJson(json, "z", RelationDef.uuNoPath));
                    editForm.set(PrivacyProfileUtil.p_tribe, PrivacyProfileUtil.getValueFromJson(json, "t", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_work, PrivacyProfileUtil.getValueFromJson(json, "w", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_blog, PrivacyProfileUtil.getValueFromJson(json, "j", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_album, PrivacyProfileUtil.getValueFromJson(json, "k", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_share, PrivacyProfileUtil.getValueFromJson(json, "n", RelationDef.uuSchoolmate));
                    editForm.set(PrivacyProfileUtil.p_gift, PrivacyProfileUtil.getValueFromJson(json, "d", RelationDef.uuSchoolmate));
                    //editForm.set(PrivacyProfileUtil.p_love, PrivacyProfileUtil.getValueFromJson(json, "x1", RelationDef.uuSelf));
                    //editForm.set(PrivacyProfileUtil.p_lover, PrivacyProfileUtil.getValueFromJson(json, "y", RelationDef.uuSelf));
                    // editForm.set(p_email, getValueFromJson(json, "o",RelationDef.uuSelf));//2010-8-1 email隐私
                    // editForm.set(p_address, getValueFromJson(json, "r", RelationDef.uuSelf));//2010-8-1 地址隐私
                    // editForm.set(p_remark, getValueFromJson(json, "u", RelationDef.uuSelf));//2010-8-1 备注隐私
                } else if (com.xiaonei.platform.core.opt.OpiConstants.isKaixin()) {
                    editForm.set(PrivacyProfileUtil.p_account, PrivacyProfileUtil.getValueFromJson(json, "a", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_friend, PrivacyProfileUtil.getValueFromJson(json, "f", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_personal, PrivacyProfileUtil.getValueFromJson(json, "p", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_status, PrivacyProfileUtil.getValueFromJson(json, "s", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_link, PrivacyProfileUtil.getValueFromJson(json, "l", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_msn, PrivacyProfileUtil.getValueFromJson(json, "m", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_qq, PrivacyProfileUtil.getValueFromJson(json, "q", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_mobiletel, PrivacyProfileUtil.getValueFromJson(json, "h", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_fixedtel, PrivacyProfileUtil.getValueFromJson(json, "i", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_class, PrivacyProfileUtil.getValueFromJson(json, "c", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_education, PrivacyProfileUtil.getValueFromJson(json, "e", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_gossip, PrivacyProfileUtil.getValueFromJson(json, "g", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_gossip_board, PrivacyProfileUtil.getValueFromJson(json, "z", RelationDef.uuNoPath));
                    editForm.set(PrivacyProfileUtil.p_tribe, PrivacyProfileUtil.getValueFromJson(json, "t", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_work, PrivacyProfileUtil.getValueFromJson(json, "w", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_blog, PrivacyProfileUtil.getValueFromJson(json, "j", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_album, PrivacyProfileUtil.getValueFromJson(json, "k", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_share, PrivacyProfileUtil.getValueFromJson(json, "n", RelationDef.uuFriend));
                    editForm.set(PrivacyProfileUtil.p_gift, PrivacyProfileUtil.getValueFromJson(json, "d", RelationDef.uuFriend));
                    // 2009-05-25,加入了follow的隐私设置
                    editForm.set(PrivacyProfileUtil.p_follow, PrivacyProfileUtil.getValueFromJson(json, "v", RelationDef.uuNoPath));
                } else {
                    System.out.println("----------domainCode is error");
                }
            } else {
                System.out.println("----------jsonObject is null=");
            }
        }
        return editForm;
    }

    public static void main(final String[] agrs) {
        final JSONObject jsonObject = PrivacyProfileUtil
        .getPpJSONObject("{a:1,c:3,e:3,f:3,g:3,l:3,m:3,p:3,q:3,s:3,t:3,w:3,h:3,i:3}");
        System.out.println(jsonObject);
        System.out.println(jsonObject.toString().replace("\"", ""));
        System.out.println(jsonObject.get("f"));
    }

    public static void saveLovePrivacy(final UserConfigInfo uc , final int privacy){
        final JSONObject jsonObject = PrivacyProfileUtil.getPpJSONObject(uc.getProfilePrivacy());
        jsonObject.put("x1", privacy);
        uc.setProfilePrivacy(jsonObject.toString().replace("\"", ""));
        SnsAdapterFactory.getUserConfigInfoAdapter().setUserConfig(uc.getId(), uc);
    }

    public static void saveLoverPrivacy(final UserConfigInfo uc, final int privacy) {
        final JSONObject jsonObject = PrivacyProfileUtil.getPpJSONObject(uc.getProfilePrivacy());
        jsonObject.put("y", privacy);
        uc.setProfilePrivacy(jsonObject.toString().replace("\"", ""));
        SnsAdapterFactory.getUserConfigInfoAdapter().setUserConfig(uc.getId(), uc);
    }

    private static String getstr1(final char c, final Object s) {
        return c + ":" + s + ",";
    }

    private static String getstr1(final String c, final Object s) {
        return c + ":" + s + ",";
    }
}
