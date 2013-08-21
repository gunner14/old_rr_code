package com.xiaonei.platform.component.gossip.view;

import xce.recentcontact.ContactInfo;

public class RecentContactView  {
	
	public RecentContactView(){
		
	}
	
	public RecentContactView(ContactInfo info){
		this.guestId = info.guestId;
	}
	
	public int getGuestId() {
		return guestId;
	}
	public void setGuestId(int guestId) {
		this.guestId = guestId;
	}
	public String getGuestName() {
		return guestName;
	}
	public void setGuestName(String guestName) {
		this.guestName = guestName;
	}

	public void setResource(int resource) {
		this.resource = resource;
	}
	public int getResource() {
		return resource;
	}
	
	private int guestId;
	private String guestName;
	private int resource;
}
