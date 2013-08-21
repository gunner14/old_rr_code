package com.dodoyo.invite.core.logic;

import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.invite.core.entity.InviteMailStat;
import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.model.Email;

public class StatUtils {
    
//    @Autowired
//    private InviteServiceDaoFacade daoFacade;
    
    @Autowired
    private BatchInsertUtils batchInsertUtils;
    
//    private static StatUtils su = new StatUtils();

    public StatUtils() {
    }

   /* private static StatUtils getInstance() {
        return su;
    }*/

    /**
     * 邀请邮件发送量统计
     *
     * @param ip
     * @param email
     * @return
     */
    public boolean recordInviteMail(InviteParams ip, Email email) {
        try {
            InviteMailStat ims = new InviteMailStat();
            ims.setSource_id(Integer.parseInt(ip.getTemplateContent().getSourceId()));
            ims.setApp_id(ip.getAppId() == null || "".equals(ip.getAppId()) ? 0 : Integer.parseInt(ip.getAppId()));
            ims.setEmail_from(ip.getImporterMail());
            ims.setEmail_to(ip.getInviteeMail());
            ims.setUserid(ip.getInviterId());
            if (ip.getInviteeMail().split("@").length == 2) {
                ims.setMail_suffix(ip.getInviteeMail().split("@")[1]);
                ims.setMailSuffixTo(ip.getInviteeMail().split("@")[1]);
            }

            if (ip.getImporterMail() != null && ip.getImporterMail().split("@").length == 2)
                ims.setMailSuffixFrom(ip.getImporterMail().split("@")[1]);

            //stat data setting
            if (ip.getStatMap() != null) {
                if (ip.getStatMap().get("invite_type") != null && !"".equals(ip.getStatMap().get("invite_type")))
                    ims.setInvite_type(Integer.parseInt(ip.getStatMap().get("invite_type")));
                if (ip.getStatMap().get("enter_type") != null && !"".equals(ip.getStatMap().get("enter_type")))
                    ims.setEnter_type(Integer.parseInt(ip.getStatMap().get("enter_type")));
                if (ip.getStatMap().get("action_type") != null && !"".equals(ip.getStatMap().get("action_type")))
                    ims.setAction_type(Integer.parseInt(ip.getStatMap().get("action_type")));
            }
//            InviteMailStatDAO.getInstance().insertRecorder(ims);
            batchInsertUtils.addIms(ims);
        }
        catch (Exception e) {
            LogUtils.logExceptionStack(e);
            return false;
        }

        return true;
	}
}