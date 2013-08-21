package com.dodoyo.invite.core.logic;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.invite.core.Constants;
import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.core.entity.InviteUrlParas;
import com.dodoyo.invite.dao.InviteServiceDaoFacade;
import com.dodoyo.invite.model.UserInviteCodeView;
import com.xiaonei.platform.component.application.utils.MD5;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.framework.exception.DataAccessException;

public class UUIDUtils {
    
    @Autowired
    private InviteServiceDaoFacade daoFacade;
    
    @Autowired
    private InviteParamsUtils inviteParamsUtils;
    
    /*@Autowired
    private BatchInsertUtils batchInsertUtils;*/
    
    /*private static UUIDUtils uu = new UUIDUtils();*/

    /*private static UUIDUtils getInstance() {
        return uu;
    }*/

    //public methods
    /**
     * 将uuid写入传入的ip对象中
     */
    public String updateInviteParamsByUUID(InviteParams ip) {
        StringBuffer sb = new StringBuffer(Constants.UUID_GEN_KEY);

        sb.append(ip.getInviterId()).append("-").
                append(ip.getAppId()).append("-").
                append(ip.getInviteeMail()).append("-").
                append(ip.getTemplateContent().getSourceId());

        String md5Key = MD5.digest(sb.toString());
        md5Key = this.convertMD5toUUID(md5Key);
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
                final InviteUrlParas iup = new InviteUrlParas();
                final String domain = OpiConstants.getCurrentDomain();
                try {
                    if (inviteParamsUtils.isRealAppId(ip.getAppId()))
                        iup.setApp_id(Integer.parseInt(ip.getAppId()));
                    else if (inviteParamsUtils.isRealAppId(ip.getTemplateContent().getAppId()))
                        iup.setApp_id(Integer.parseInt(ip.getTemplateContent().getAppId()));
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                iup.setEmail_to(ip.getInviteeMail());
                iup.setMd5(md5Key);
                iup.setUuid(md5Key);
                iup.setSs(ip.getTemplateContent().getSourceId());
                iup.setEmail_from(ip.getImporterMail());
                iup.setUser_id(ip.getInviterId());
                iup.setApp_token(ip.getAppToken());
                iup.setRt(ip.getTemplateContent().getRt());
                
                /**insert invite_url_paras_**/
                Runnable task = new Runnable(){
                    public void run(){
                        try{
                            OpiConstants.setCurrentDomain(domain);
                            //iup.setId(daoFacade.getSequenceDAO().getSequenceId());//one view one get, too bad
                            daoFacade.getInviteUrlParasDAO().insertIup(iup);
                        }
                        catch (DataAccessException e){
                            e.printStackTrace(System.err);
                        }
                    }
                };
                RunnableTaskExecutor.getInstance().runTask(task);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        return md5Key;
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
        StringBuffer sb = new StringBuffer(Constants.UUID_GEN_KEY);
        sb.append(ip.getInviterId()).append("-").
                append(appId).append("-").
                append(ip.getBuddyGroup()).append("-").
                append(linkSs).append("-").
                append(rt);
        String md5Key = MD5.digest(sb.toString());
        md5Key = this.convertMD5toUUID(md5Key);

        try {
                //user code handle
                UserInviteCodeView view = null;
                try {
                    view = getCodeView(/*ip.getInviterId()*/);
                } catch (Exception e) {
                    e.printStackTrace(System.err);
                }

                final InviteUrlParas iup = new InviteUrlParas();
                final String domain = OpiConstants.getCurrentDomain();
                if (ip.getAppId() != null && !"".equals(ip.getAppId()))
                    iup.setApp_id(Integer.parseInt(ip.getAppId()));
                iup.setEmail_to(ip.getInviteeMail());
                iup.setMd5(md5Key);
                iup.setUuid(md5Key);
                iup.setSs(linkSs);
                iup.setRt(rt);
                iup.setUser_id(ip.getInviterId());
                iup.setApp_token(ip.getAppToken());
                iup.setCode(view == null ? "default" : view.getInviteCode());
                iup.setBuddy_group(ip.getBuddyGroup());
                
                /**insert invite_url_paras_**/
                Runnable task = new Runnable(){
                    public void run(){
                        try{
                            OpiConstants.setCurrentDomain(domain);
                            //iup.setId(daoFacade.getSequenceDAO().getSequenceId());
                            daoFacade.getInviteUrlParasDAO().insertIup(iup);
                        }
                        catch (DataAccessException e){
                            e.printStackTrace(System.err);
                        }
                    }
                };
                RunnableTaskExecutor.getInstance().runTask(task);
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        return md5Key;
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

    private UserInviteCodeView getCodeView(/*int inviterId*/) throws DataAccessException {
        return null;
        //if (true) return null;//cancel view code generation
//        UserInviteCodeView uic = new UserInviteCodeViewDAO().getByUser(inviterId);
        /*UserInviteCodeView uic = daoFacade.getUserInviteCodeViewDAO().getByUser(inviterId);
        //System.out.println("3------------------>" + uic.getInviteCode());
        if (uic == null) {
            uic = new UserInviteCodeView();
            uic.setId(inviterId);
            do {
                uic.setInviteCode(VerifyCode.getVerifyCode(10));
//            } while (new UserInviteCodeViewDAO().getByCode(uic.getInviteCode()) != null);
            } while (daoFacade.getUserInviteCodeViewDAO().getByCode(uic.getInviteCode()) != null);
            LogUtils.logDebug("0------------------>" + uic.getInviteCode());
//            UserPersonalInfoDAO.getInstance().insertInviteCode(uic.getId(), uic.getInviteCode());
            daoFacade.getUserPersonalInfoDAO().insertInviteCode(uic.getId(), uic.getInviteCode());
        } else {
            if (uic.getInviteCode() == null) {
                do {
                    uic.setInviteCode(VerifyCode.getVerifyCode(10));
//                } while (new UserInviteCodeViewDAO().getByCode(uic.getInviteCode()) != null);
                } while (daoFacade.getUserInviteCodeViewDAO().getByCode(uic.getInviteCode()) != null);
                LogUtils.logDebug("------------------>" + uic.getInviteCode());
//                UserPersonalInfoDAO.getInstance().updateInviteCode(uic.getId(), uic.getInviteCode());
                daoFacade.getUserPersonalInfoDAO().updateInviteCode(uic.getId(), uic.getInviteCode());
            }
        }
        return uic;*/
    }
    
    private String convertMD5toUUID(String md5)
    {
        if (md5 == null || md5.length() != 32)
            return "";
        StringBuilder sb = new StringBuilder("");
        sb.append(md5.substring(0, 8));
        sb.append("-");
        sb.append(md5.substring(8, 12));
        sb.append("-");
        sb.append(md5.substring(12, 16));
        sb.append("-");
        sb.append(md5.substring(16, 20));
        sb.append("-");
        sb.append(md5.substring(20, 32));
        
        return sb.toString();
    }
    
    public static void main(String[] args) {
//		String linkUUID = UUIDUtils.getInstance().genUUIDForLinkInviteAll(10002, "123", "分组", "", "");
//		LogUtils.logDebug("uuid for link return is: " + linkUUID);

//        Map<String, String> map = UUIDUtils.getInstance().genUUIDForLinkInviteAll(10002, "");
        /*for (String key : map.keySet())
            System.out.println("uuid for link return is: " + map.get(key));*/
//        for (Entry<String, String> e : map.entrySet())
//            System.out.println("uuid for link return is: " + e.getValue());
//        int sno = new UUIDUtils().getInviteUrlTableSno("565656ff");
//        System.out.println(sno);
        long dt = System.currentTimeMillis();
        String uuid = new UUIDUtils().convertMD5toUUID(MD5.digest("test"));
        System.out.println(uuid + " " + (System.currentTimeMillis()-dt));
	}
}