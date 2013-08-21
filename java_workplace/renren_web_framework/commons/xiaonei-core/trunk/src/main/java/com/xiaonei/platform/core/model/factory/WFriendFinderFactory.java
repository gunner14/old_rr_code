package com.xiaonei.platform.core.model.factory;

import com.xiaonei.platform.core.model.WFriendCommon;
import com.xiaonei.platform.core.model.WFriendShare;

import mop.hi.oce.domain.factory.FriendFinderFactory;
import mop.hi.oce.domain.model.FriendCommon;
import mop.hi.oce.domain.model.FriendShare;

public class WFriendFinderFactory implements FriendFinderFactory{

	public FriendCommon createFriendCommon() {
		// TODO Auto-generated method stub
		 return new WFriendCommon();
	}

	public FriendShare createFriendShare() {
		// TODO Auto-generated method stub
		return new WFriendShare();
	}

}
