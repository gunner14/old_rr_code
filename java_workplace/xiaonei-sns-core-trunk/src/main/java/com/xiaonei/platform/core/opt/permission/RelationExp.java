package com.xiaonei.platform.core.opt.permission;


import mop.hi.oce.domain.buddy.BuddyRelation;

public interface  RelationExp {

    public  boolean isRequestFriend(BuddyRelation guesterToOwner) ;
	public  boolean isRequestFriended(BuddyRelation guesterToOwner) ;
	public  boolean isBlocked(BuddyRelation guesterToOwner) ;
	public  boolean isBlock(BuddyRelation guesterToOwner) ;
	public  boolean isSamer(int guester,int owner); 
	public  boolean isFriend(BuddyRelation guesterToOwner);
	public  boolean isSameNet(Source guester,Source source);
	public  boolean isSameStage(Source guester,Source source);
	public int getRelationPath(Source guester,Source source);
	public  boolean  isSendConfirmed(int guester,int owner);
	public boolean isGouYinGuo(Source guester,Source source) ;
	
}
