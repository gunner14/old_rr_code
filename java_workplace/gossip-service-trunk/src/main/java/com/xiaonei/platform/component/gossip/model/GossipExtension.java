/**
 * 
 */
package com.xiaonei.platform.component.gossip.model;

/**
 * @author 康伟
   2009-9-13
 */
public class GossipExtension {
    private long id;
    private String head_url;
    private String large_url;
    private int owner;

    public int getOwner() {
		return owner;
	}

	public void setOwner(int owner) {
		this.owner = owner;
	}

	public long getId() {
	return id;
    }

    public void setId(long id) {
	this.id = id;
    }

    public String getHead_url() {
	return head_url;
    }

    public void setHead_url(String head_url) {
	this.head_url = head_url;
    }

    public String getLarge_url() {
	return large_url;
    }

    public void setLarge_url(String large_url) {
	this.large_url = large_url;
    }
}
