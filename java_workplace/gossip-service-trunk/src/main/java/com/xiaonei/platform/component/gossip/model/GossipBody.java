package com.xiaonei.platform.component.gossip.model;

import java.io.Serializable;
import java.util.Date;

import com.xiaonei.platform.component.gossip.biz.GossipBiz;

/**
 * gossip_body表和gossip_body_recent_x表对应的model对象
 * 
 * @author lifs
 */
public class GossipBody implements Serializable {
	private static final long serialVersionUID = -4641882370921156089L;
	private long id;
    private String body;
    private Date time;
    
	public String getBody() {
		return body;
	}
	public void setBody(String body) {
		this.body = body;
	}
	public long getId() {
		return id;
	}
	public void setId(long id) {
		this.id = id;
	}
	public Date getTime() {
		return time;
	}
	public void setTime(Date time) {
		this.time = time;
	}   

    public  String getFilterOriginalBody(){
    	return GossipBiz.getFilterOriginalBody(body);
    }
    
}