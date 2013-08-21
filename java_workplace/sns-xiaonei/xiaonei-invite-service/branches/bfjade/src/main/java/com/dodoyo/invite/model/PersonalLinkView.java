package com.dodoyo.invite.model;

import java.io.Serializable;

public class PersonalLinkView implements Serializable {

    private static final long serialVersionUID = -801759533511791252L;
    public static final int statusOpen = 1;
    public static final int statusClose = 0;
    private int id;
    private String link;
    private int status;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getLink() {
        return link;
    }

    public void setLink(String link) {
        this.link = link;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }
}
