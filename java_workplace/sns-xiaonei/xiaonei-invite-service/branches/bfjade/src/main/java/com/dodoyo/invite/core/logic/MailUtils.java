package com.dodoyo.invite.core.logic;

import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.dao.EmailDAO;
import com.dodoyo.invite.model.Email;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

import java.sql.SQLException;

public class MailUtils {
    private MailUtils() {
    }

    private static MailUtils mu = new MailUtils();

    public static MailUtils getInstance() {
        return mu;
    }

    /**
     * 纯粹的发邀请信，入库
     *
     * @param email
     * @return
     */
    public boolean sendMail(Email email) {
        //if (true) return true;//线上测试2009-3-31 by Chris Gao
        EmailDAO ed = new EmailDAO();
        try {
            ed.save(email);
        }
        catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
        finally {
            ed = null;//destroy
        }
        return true;
    }

    /**
     * core method for mail content got and parse all available params
     *
     * @param ip
     * @return
     */
    public Email getInviteEmail(InviteParams ip) {
        String mailTitle = ip.getTemplateContent().getTitle();
        mailTitle = parseSimpleContent(mailTitle, ip);
        ip.getTemplateContent().setTitle(mailTitle);

        String mailBody = ip.getTemplateContent().getBody();

        LogUtils.logDebug("ip = " + ip);

        LogUtils.logDebug("before parseSimpleContent, mailBody = " + mailBody);

        mailBody = parseSimpleContent(mailBody, ip);

        LogUtils.logDebug("after parseSimpleContent, mailBody = " + mailBody);

        ip.getTemplateContent().setBody(mailBody);

        LogUtils.logDebug("before VM ###########mailBody = " + mailBody);

        mailBody = TemplateUtils.getInstance().getVMContent(InviteParamsUtils.getInstance().getDatasForVMReplace(ip),
                TemplateUtils.getInstance().getVMTemplate(ip.getTemplateContent().getVmTemplate()));

        LogUtils.logDebug("after VM ###########mailBody = " + mailBody);

        String mailFrom = getMailSender(ip);

        LogUtils.logDebug("###########mailFrom = " + mailFrom);

        Email email = new Email(ip.getInviteeMail(), mailFrom, mailTitle, mailBody);

        email.setDisc(Integer.parseInt(ip.getTemplateContent().getDisc()));

        return email;
    }

    private String parseSimpleContent(String content, InviteParams ip) {
        User inviter = WUserAdapter.getInstance().get(ip.getInviterId());
        LogUtils.logDebug("in parseSimpleContent, invterName = " + inviter.getName());

        String newContent = content.replaceAll("#name#", inviter.getName());
        LogUtils.logDebug("in parseSimpleContent newContent = " + newContent);
        return newContent;
    }

    private String getMailSender(InviteParams ip) {
        User inviter = WUserAdapter.getInstance().get(ip.getInviterId());

        String mailSender = ip.getTemplateContent().getSender();
        if (mailSender == null || "".equals(mailSender))
            mailSender = inviter.getName() + ";" + inviter.getEmail();
		
		return mailSender;
	}
}