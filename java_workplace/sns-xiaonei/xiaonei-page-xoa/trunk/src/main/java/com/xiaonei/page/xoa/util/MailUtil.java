package com.xiaonei.page.xoa.util;

import java.sql.SQLException;
import java.util.List;

import com.xiaonei.admin.biz.model.invite.Email;
public class MailUtil {

    private String subject;

    private String content;

    private static String From = "guangsheng.liu@renren-inc.com";
    
    private static final String[] SuccessAlertTo = new String[] {
            "guangsheng.liu@renren-inc.com" };

    private static final String[] ExceptionAlertTo = SuccessAlertTo;

    
    public MailUtil(String subject, String content) {
        this.subject = subject;
        this.content = content;
    }
    
    /**
     * 发送失败信息的邮件。
     * 
     * @throws SQLException
     */
    public void sendExceptionAlert() throws SQLException {
        sendTo(ExceptionAlertTo);
    }
    /**
     * 
     * 发送邮件。
     * 
     * @param mailTo
     * @throws SQLException
     */
    public void sendTo(String mailTo) throws SQLException {
        Email em = new Email(mailTo, From, this.subject, this.content);
        em.setDisc(10000010);
        EmailDAO.getInstance().save(em);
    }
    /**
     * 
     * 发送到邮件列表。
     * 
     * @param mailList
     * @throws SQLException
     */
    public void sendTo(String[] mailList) throws SQLException {
        for (String to : mailList) {
            sendTo(to);
        }
    }
    /**
     * 
     * 发送到邮件列表。
     * 
     * @param mails
     * @throws SQLException
     */
    public void sendTo(List<String> mails)throws SQLException{
        for (String mail : mails) {
            sendTo(mail);
        }
    }
    /**
     * 
     * 发送默认的成功邮件。
     * 
     * @throws SQLException
     */
    public void sendSuccessAlert() throws SQLException {
        sendTo(SuccessAlertTo);
    }
}
