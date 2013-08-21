package com.dodoyo.invite.core.logic;

import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.core.facade.InviteFacade;
import com.dodoyo.invite.model.Email;
import com.xiaonei.platform.core.opt.OpiConstants;

public class InviteMailLogic {
    private static InviteMailLogic iml = new InviteMailLogic();

    private InviteMailLogic() {
    }

    public static InviteMailLogic getInstance() {
        return iml;
    }


    //public methods
    public String sendInviteMail(final InviteParams ip) {
        //check InviteParams is valid or not
        boolean isIpValid = InviteParamsUtils.getInstance().checkInviteParams(ip);
        if (!isIpValid)
            return "";

        //update InviteParams by template content
        TemplateUtils.getInstance().updateInviteParamsByTemplateId(ip);

        long dt = System.currentTimeMillis();
        
        final String currentDomain = OpiConstants.getCurrentDomain();
        
        Runnable fourSteps = new Runnable() {
            public void run() {
            	OpiConstants.setCurrentDomain(currentDomain);
                //update other params
                InviteParamsUtils.getInstance().updateInviteParamsForPrepare(ip);
                LogUtils.logDebug("ip.getAppToken() = " + ip.getAppToken());

                //update uuid
                UUIDUtils.getInstance().updateInviteParamsByUUID(ip);
                LogUtils.logDebug(ip.getUuid());

                //get email object
                Email email = MailUtils.getInstance().getInviteEmail(ip);
                LogUtils.logDebug(email.getBody());
                LogUtils.logDebug(email.getEmailFrom());
                LogUtils.logDebug(email.getTitle());

                //send invite mail here
                MailUtils.getInstance().sendMail(email);

                //add invite mail statistic here
                StatUtils.getInstance().recordInviteMail(ip, email);
            }
        };
        RunnableTaskExecutor.getInstance().runTask(fourSteps);

        LogUtils.logDebug("$$$$$$four steps" + (System.currentTimeMillis() - dt));

        return ip.getTemplateContent().getSourceId();
    }

    public static void main(String[] args) {
//		InviteParams ip = new InviteParams();
//		ip.setInviteeMail("111hongliang.gao@kaixin.com");
//		ip.setInviterId(400000458);
//		ip.setTemplateId("208052");
//		ip.setAppId("19");
//		InviteMailLogic.getInstance().sendInviteMail(ip);

        //InviteFacade.getInviteFacade().sendInviteMail(400000458,"28", "gaohongliang@email.com.cn");
        String url = InviteFacade.getInviteFacade().getURLForLinkInvite(400000458, "123324", "grouptest", "", "");
        LogUtils.logDebug(url);

//		InviteFacade.getInviteFacade().sendInviteMail(400000458, "19", "20805", "hongliang.gao11223@test.com");
//		InviteFacade.getInviteFacade().sendInviteMail(400000458, "","","test123@sohu.com");

//		String str = LinkConfUtils.getInstance().getAppRt();
//		LogUtils.logInfo(str);
    }
}