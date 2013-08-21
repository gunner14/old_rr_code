package com.xiaonei.platform.core.model;

import java.io.Serializable;
import java.util.Date;

/**
 * 
 * DailyEmail.java
 * Created by @author freebat (freebat@gmail.com) at 2005-12-21 15:46:20
 */
public class DailyEmail implements Serializable {
	private static final long serialVersionUID = -2619366160629509431L;
	public static final int notifyNo = 0;
	public static final int notifyYes = 1;

	private int id;
    private int owner;
    private int disc;
    private String body;
    private String subject;
    private Date time;
    private Date uptime;
    
    public DailyEmail(int host) {
    	super();
    	this.owner = host;
        time = new Date();
        uptime = new Date();
    }

	public String getBody() {
		return body;
	}

	public void setBody(String body) {
		this.body = body;
	}

	public int getDisc() {
		return disc;
	}

	public void setDisc(int disc) {
		this.disc = disc;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getSubject() {
		return subject;
	}

	public void setSubject(String subject) {
		this.subject = subject;
	}

	public Date getTime() {
		return time;
	}

	public void setTime(Date time) {
		this.time = time;
	}

	public Date getUptime() {
		return uptime;
	}

	public void setUptime(Date uptime) {
		this.uptime = uptime;
	}

	public int getOwner() {
		return owner;
	}

	public void setOwner(int owner) {
		this.owner = owner;
	}
}