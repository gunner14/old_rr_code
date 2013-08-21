package com.xiaonei.reg.register.model;

import java.io.Serializable;
import java.util.Date;

/**
 * Email.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-23 下午08:58:20
 */
public class Email implements Serializable {
    private static final long serialVersionUID = -2619366160629509431L;
    private int id;
    private String body;
    private String title;
    private String emailTo;
    private String emailFrom;
    private Date addtime;
    private int disc;

    public static final int MSN_INVITE_MAIL = 1; //临时改变成2
    public static final int GMAIL_INVITE_MAIL = 2;
    public static final int OUTLOOK_INVITE_MAIL = 3;

    public Email(String emailTo, String emailFrom, String title, String body) {
        super();
        this.addtime = new Date();
        this.title = title;
        this.body = body;
        this.emailTo = emailTo;
        this.emailFrom = emailFrom;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getEmailTo() {
        return emailTo;
    }

    public void setEmailTo(String emailTo) {
        this.emailTo = emailTo;
    }

    public Date getAddtime() {
        return addtime;
    }

    public void setAddtime(Date addtime) {
        this.addtime = addtime;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getEmailFrom() {
        return emailFrom;
    }

    public void setEmailFrom(String emailFrom) {
        this.emailFrom = emailFrom;
    }

    public int getDisc() {
        return disc;
    }

    public void setDisc(int disc) {
        this.disc = disc;
    }
}