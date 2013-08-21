package com.xiaonei.platform.core.opt.permission.strategy.impl;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.permission.AbcStrategy;
import com.xiaonei.platform.core.opt.permission.Source;
import com.xiaonei.platform.core.opt.permission.Strategy;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.source.Status;
import com.xiaonei.platform.core.opt.permission.strategy.Ret;
import com.xiaonei.xce.friend.FriendAdapter;

public class FriendFriend extends AbcStrategy {

	public Ret doStrategy(Source guester, Source source) {
		if (OpiConstants.isKaixin()) {
			Status guest = (Status) guester;
			Status owner = (Status) source;
			if (owner.getControl() == RelationDef.uuFriendFriend) {
				boolean isfriend = FriendAdapter.hasShareFriends(guest.getUserId(), owner
						.getUserId());
				//System.out.println("zrj: call hasShareFriends result:"+isfriend);
				return  isfriend ? Strategy.RET_TRUE : Strategy.RET_CONTINUE;
			}
		} 
		return Strategy.RET_CONTINUE;
	}

}
