package com.xiaonei.platform.component.gossip.model;

import java.io.Serializable;
import java.util.Date;


/**
 * gossip表和gossip_recent_x表对应的model对象
 *  
 * @author lifs
 *
 */
public class Gossip implements Serializable {
	private static final long serialVersionUID = 9146857822142668224L;
	private long id;
    private int owner;
    private String name;
    private int sender;
    private int chatId;
	private String tinyurl;
    private String univ_name;
    private GossipBody body;
    
    private String headUrl;
    private String large_url;
    
//    //@提醒信息
//    private MentionInfo mentionInfo;
    
    public int getChatId() {
		return chatId;
	}

	public void setChatId(int chatId) {
		this.chatId = chatId;
	}
    
    public String getHeadUrl() {
		return headUrl;
	}

	public void setHeadUrl(String headUrl) {
		this.headUrl = headUrl;
	}

	public String getLarge_url() {
		return large_url;
	}

	public void setLarge_url(String large_url) {
		this.large_url = large_url;
	}

	public Gossip(int owner, int sender, String body) {
    	super();
    	this.owner = owner;
    	this.sender = sender;
    	this.body = new GossipBody();
    	this.body.setBody(body);
    	this.body.setTime(new Date());
    }
    
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

	public int getOwner() {
		return owner;
	}

	public void setOwner(int owner) {
		this.owner = owner;
	}

	public int getSender() {
		return sender;
	}

	public void setSender(int sender) {
		this.sender = sender;
	}

	public GossipBody getBody() {
		return body;
	}

	public void setBody(GossipBody body) {
		this.body = body;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}


	public String getTinyurl() {
		return tinyurl;
	}

	public void setTinyurl(String tinyurl) {
		this.tinyurl = tinyurl;
	}

	public String getUniv_name() {
		return univ_name;
	}

	public void setUniv_name(String univ_name) {
		this.univ_name = univ_name;
	}

//    public MentionInfo getMentionInfo() {
//        return mentionInfo;
//    }
//
//    public void setMentionInfo(MentionInfo mentionInfo) {
//        this.mentionInfo = mentionInfo;
//    }
	
}