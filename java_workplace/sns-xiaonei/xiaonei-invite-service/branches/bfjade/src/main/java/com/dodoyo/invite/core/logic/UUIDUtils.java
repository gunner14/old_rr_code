package com.dodoyo.invite.core.logic;

import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.core.entity.InviteUrlParas;
import com.dodoyo.invite.dao.InviteUrlParasDAO;
import com.dodoyo.invite.dao.UserInviteCodeViewDAO;
import com.dodoyo.invite.dao.UserPersonalInfoDAO;
import com.dodoyo.invite.model.UserInviteCodeView;
import com.xiaonei.platform.component.application.utils.MD5;
import com.xiaonei.platform.core.utility.VerifyCode;

import java.sql.SQLException;
import java.util.*;

public class UUIDUtils {
    private static UUIDUtils uu = new UUIDUtils();

    public static UUIDUtils getInstance() {
        return uu;
    }

    //public methods
    /**
     * 将uuid写入传入的ip对象中
     */
    public String updateInviteParamsByUUID(InviteParams ip) {
        StringBuffer sb = new StringBuffer();

        sb.append(ip.getInviterId()).append("-").
                append(ip.getAppId()).append("-").
                append(ip.getInviteeMail()).append("-").
                append(ip.getTemplateContent().getSourceId());

        String md5Key = MD5.digest(sb.toString());
        LogUtils.logDebug("md5Key = " + md5Key);

        String uuid = getParasUUID(md5Key, ip);
        LogUtils.logDebug("uuid = " + uuid);

        ip.setUuid(uuid);

        return uuid;
    }

    /**
     * DB不存在则直接生成新的UUID返回，存在则从DB返回UUID
     *
     * @param md5Key
     * @return
     */
    private String getParasUUID(String md5Key, InviteParams ip) {
        try {
            List<InviteUrlParas> list = InviteUrlParasDAO.getInstance().getInviteUrlParasByMd5(md5Key);
            if (list == null || list.size() == 0) {
                String newUUID = genNewUUID();
                InviteUrlParas iup = new InviteUrlParas();
                try {
                    if (InviteParamsUtils.getInstance().isRealAppId(ip.getAppId()))
                        iup.setApp_id(Integer.parseInt(ip.getAppId()));
                    else if (InviteParamsUtils.getInstance().isRealAppId(ip.getTemplateContent().getAppId()))
                        iup.setApp_id(Integer.parseInt(ip.getTemplateContent().getAppId()));
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                iup.setEmail_to(ip.getInviteeMail());
                iup.setMd5(md5Key);
                iup.setUuid(newUUID);
                iup.setSs(ip.getTemplateContent().getSourceId());
                iup.setEmail_from(ip.getImporterMail());
                iup.setUser_id(ip.getInviterId());
                iup.setApp_token(ip.getAppToken());
                iup.setRt(ip.getTemplateContent().getRt());
                InviteUrlParasDAO.getInstance().InsertUser(iup);
                return newUUID;
            }
            LogUtils.logDebug("uuid with key = " + md5Key + " is exist ....");
            return list.get(0).getUuid();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        return genNewUUID();
    }

    private String getSysDefaultLinkSs(String appId) {
        if (appId.equals("0"))
            return LinkConfUtils.getInstance().getLinkSs("其他");
        else
            return LinkConfUtils.getInstance().getAppLinkSs("其他");
    }

    /**
     * 根据传入的参数，返回连接邀请所需的UUID
     *
     * @param ip
     * @return
     */
    private String genUUIDForLinkInvite(InviteParams ip) {
        //appId handle
        String appId = ip.getAppId();
        if (appId == null || "".equals(appId))
            appId = "0";

        //user code handle
//		UserInviteCodeView  view = null;
//		try{
//			view = getCodeView(ip.getInviterId());
//		}catch(Exception e){
//			e.printStackTrace(System.err);
//		}

        //get ss, rt
        String linkSs = null;
        String rt = null;

        if (appId.equals("0")) {
            linkSs = ip.getSpecSs() == null || "".equals(ip.getSpecSs()) ? LinkConfUtils.getInstance().getLinkSs(ip.getBuddyGroup()) : ip.getSpecSs();
            rt = ip.getSpecRt() == null || "".equals(ip.getSpecRt()) ? LinkConfUtils.getInstance().getRt() : ip.getSpecRt();
        } else {
            linkSs = ip.getSpecSs() == null || "".equals(ip.getSpecSs()) ? LinkConfUtils.getInstance().getAppLinkSs(ip.getBuddyGroup()) : ip.getSpecSs();
            rt = ip.getSpecRt() == null || "".equals(ip.getSpecRt()) ? LinkConfUtils.getInstance().getAppRt() : ip.getSpecRt();
        }
        if (linkSs == null)
            linkSs = getSysDefaultLinkSs(appId);


        //generate md5 key
        StringBuffer sb = new StringBuffer("");
        sb.append(ip.getInviterId()).append("-").
                append(appId).append("-").
                append(ip.getBuddyGroup()).append("-").
                append(linkSs).append("-").
                append(rt);
        String md5Key = MD5.digest(sb.toString());

        try {
            List<InviteUrlParas> list = InviteUrlParasDAO.getInstance().getInviteUrlParasByMd5(md5Key);
            if (list == null || list.size() == 0) {
                //user code handle
                UserInviteCodeView view = null;
                try {
                    view = getCodeView(ip.getInviterId());
                } catch (Exception e) {
                    e.printStackTrace(System.err);
                }

                String newUUID = genNewUUID();
                InviteUrlParas iup = new InviteUrlParas();
                if (ip.getAppId() != null && !"".equals(ip.getAppId()))
                    iup.setApp_id(Integer.parseInt(ip.getAppId()));
                iup.setEmail_to(ip.getInviteeMail());
                iup.setMd5(md5Key);
                iup.setUuid(newUUID);
                iup.setSs(linkSs);
                iup.setRt(rt);
                iup.setUser_id(ip.getInviterId());
                iup.setApp_token(ip.getAppToken());
                iup.setCode(view == null ? "default" : view.getInviteCode());
                iup.setBuddy_group(ip.getBuddyGroup());
                InviteUrlParasDAO.getInstance().InsertUser(iup);
                return newUUID;
            }
            LogUtils.logDebug("link uuid with key = " + md5Key + " is exist ....");
            return list.get(0).getUuid();
        }
        catch (SQLException e) {
            e.printStackTrace();
        }

        return genNewUUID();
    }

    public String genUUIDForLinkInvite(int inviterId, String appId, String groupName, String ss, String rt) {
        InviteParams ip = new InviteParams();
        ip.setInviterId(inviterId);
        ip.setAppId(appId);
        ip.setBuddyGroup(groupName);
        ip.setSpecSs(ss);
        ip.setSpecRt(rt);
        return genUUIDForLinkInvite(ip);
    }

    /**
     * 返回所有的链接邀请地址
     *
     * @param inviterId
     * @param appId
     * @param groupName
     * @param ss
     * @param rt
     * @return
     */
    public Map<String, String> genUUIDForLinkInviteAll(int inviterId, String appId) {
        Map<String, String> linkMap = new HashMap<String, String>();

        Set<String> keySet = null;
        if (appId == null || "".equals(appId) || "0".equals(appId))
            keySet = LinkConfUtils.getInstance().getLinkSsMap().keySet();
        else
            keySet = LinkConfUtils.getInstance().getAppLinkSsMap().keySet();

        for (String key : keySet) {
            //邀请地址 + UUID
            linkMap.put(key, LinkConfUtils.getInstance().getInviteBaseLink() +
                    genUUIDForLinkInvite(inviterId, appId, key, "", ""));
        }

        return linkMap;
    }

    private String genNewUUID() {
        return UUID.randomUUID().toString();
    }

    private UserInviteCodeView getCodeView(int inviterId) throws SQLException {
        if (true) return null;//cancel view code generation
        UserInviteCodeView uic = new UserInviteCodeViewDAO().getByUser(inviterId);
        //System.out.println("3------------------>" + uic.getInviteCode());
        if (uic == null) {
            uic = new UserInviteCodeView();
            uic.setId(inviterId);
            do {
                uic.setInviteCode(VerifyCode.getVerifyCode(10));
            } while (new UserInviteCodeViewDAO().getByCode(uic.getInviteCode()) != null);
            LogUtils.logInfo("0------------------>" + uic.getInviteCode());
            UserPersonalInfoDAO.getInstance().insertInviteCode(uic.getId(), uic.getInviteCode());
        } else {
            if (uic.getInviteCode() == null) {
                do {
                    uic.setInviteCode(VerifyCode.getVerifyCode(10));
                } while (new UserInviteCodeViewDAO().getByCode(uic.getInviteCode()) != null);
                LogUtils.logInfo("------------------>" + uic.getInviteCode());
                UserPersonalInfoDAO.getInstance().updateInviteCode(uic.getId(), uic.getInviteCode());
            }
        }
        return uic;
    }

    public static void main(String[] args) {
//		String linkUUID = UUIDUtils.getInstance().genUUIDForLinkInviteAll(10002, "123", "分组", "", "");
//		LogUtils.logDebug("uuid for link return is: " + linkUUID);

        Map<String, String> map = UUIDUtils.getInstance().genUUIDForLinkInviteAll(10002, "");
        for (String key : map.keySet())
            System.out.println("uuid for link return is: " + map.get(key));
	}
}