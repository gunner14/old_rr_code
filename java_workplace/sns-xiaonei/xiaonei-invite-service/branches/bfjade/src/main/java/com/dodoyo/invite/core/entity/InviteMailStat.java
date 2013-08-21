package com.dodoyo.invite.core.entity;

import java.io.Serializable;

/**
 * 邮件邀请的统计
 *
 * @author dream
 */
public class InviteMailStat implements Serializable {

    /**
     *
     */
    private static final long serialVersionUID = 1589210844301496403L;

    private int id = 0;

    private int userid = 0;

    private String email_from;

    private String email_to;

    private int app_id = 0;

    private int source_id = 0;

    private int action_type = 0;

    private String mail_suffix;

    private String mailSuffixFrom;
    private String mailSuffixTo;

    private int enter_type = 0;

    private int invite_type = 0;

    private String s1;

    private String s2;

    public String getMail_suffix() {
        return mail_suffix;
    }

    public void setMail_suffix(String mail_suffix) {
        this.mail_suffix = mail_suffix;
    }

    public int getEnter_type() {
        return enter_type;
    }

    public void setEnter_type(int enter_type) {
        this.enter_type = enter_type;
    }

    public int getInvite_type() {
        return invite_type;
    }

    public void setInvite_type(int invite_type) {
        this.invite_type = invite_type;
    }

    public String getS1() {
        return s1;
    }

    public void setS1(String s1) {
        this.s1 = s1;
    }

    public String getS2() {
        return s2;
    }

    public void setS2(String s2) {
        this.s2 = s2;
    }

    public String getS3() {
        return s3;
    }

    public void setS3(String s3) {
        this.s3 = s3;
    }

    public String getS4() {
        return s4;
    }

    public void setS4(String s4) {
        this.s4 = s4;
    }

    private String s3;

    private String s4;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserid() {
        return userid;
    }

    public void setUserid(int userid) {
        this.userid = userid;
    }

    public String getEmail_from() {
        return email_from;
    }

    public void setEmail_from(String email_from) {
        this.email_from = email_from;
    }

    public String getEmail_to() {
        return email_to;
    }

    public void setEmail_to(String email_to) {
        this.email_to = email_to;
    }

    public int getApp_id() {
        return app_id;
    }

    public void setApp_id(int app_id) {
        this.app_id = app_id;
    }

    public int getSource_id() {
        return source_id;
    }

    public void setSource_id(int source_id) {
        this.source_id = source_id;
    }

    public int getAction_type() {
        return action_type;
    }

    public void setAction_type(int action_type) {
        this.action_type = action_type;
    }

    public static long getSerialVersionUID() {
        return serialVersionUID;
    }

    public String getMailSuffixFrom() {
        return mailSuffixFrom;
    }

    public void setMailSuffixFrom(String mailSuffixFrom) {
        this.mailSuffixFrom = mailSuffixFrom;
    }

    public String getMailSuffixTo() {
        return mailSuffixTo;
    }

    public void setMailSuffixTo(String mailSuffixTo) {
        this.mailSuffixTo = mailSuffixTo;
    }

}
