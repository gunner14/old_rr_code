package com.renren.renrengame.utils.buddy;

import java.util.List;

import org.apache.log4j.Logger;

import com.renren.renrengame.utils.RRGameConstants;
import com.xiaonei.platform.component.friends.model.FriendApply;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

import mop.hi.oce.domain.buddy.BuddyApplyWithInfo;

public class BuddyHandler {
	private final Logger log = Logger.getLogger(BuddyHandler.class.getName());
	private String message = "";
	
	public List<Integer> getFriendList(int userId, int limit)
	{
		return com.xiaonei.xce.buddyadapter.buddybyidcache.BuddyByIdCacheAdapter.getInstance().getFriendListAsc(userId, limit); 
	}
	
	public void apply(int applicant, int accepter, String game)
	{		 
		mop.hi.oce.adapter.BuddyCoreAdapter buddyApply = new mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl();
		BuddyApplyWithInfo ba = new  BuddyApplyWithInfo(applicant,  accepter);
//		AAAA通过人人游戏大厅中XX游戏 找到您，并想添加您为好友。
		String content = /*WUserAdapter.getInstance().get(applicant).getName()+*/"通过人人游戏大厅找到您，想和您一起玩《"+game+"》";
		log.debug("BuddyHandler.apply ---> applicant:"+applicant+"  accepter:"+accepter+"   game:"+game
				+"   content:"+content);
		ba.setWhy(content);
		buddyApply.addApply(ba, RRGameConstants.ICODE);
	}
	public int applyBuddy(int applicant, int accepter, String game)
	{
		WUserAdapter wa  = WUserAdapter.getInstance();
		User appUser = wa.get(applicant);
		if (appUser == null)
			return -1;
		User accUser = wa.get(accepter);
		if (accUser == null)
			return -1;
		 
		FriendApply fa = new FriendApply(appUser,accUser,"www.renren.com");
	//	String content = wa.get(applicant).getName()+"通过人人游戏大厅中"+game+"游戏找到您，并想添加您为好友。";
		String content = /*WUserAdapter.getInstance().get(applicant).getName()+*/"通过人人游戏大厅找到您，想和您一起玩"; 
		if (game.length() > 0)
			content += "《"+game+"》";	
		fa.setCode(RRGameConstants.ICODE);
		fa.setContent(content);		
		
		log.debug("BuddyHandler.apply ---> applicant:"+applicant+"  accepter:"+accepter+"   game:"+game
				+"   content:"+content);
		
		com.xiaonei.platform.component.friends.model.FriendApplyResult far= null;
		try {
			far = com.xiaonei.platform.component.friends.home.FriendsHome.getInstance().requestFriendApply(fa);
		} catch (Exception e) {
			e.printStackTrace();
			message = "申请好友时发生异常";
			return -1;
		}
		
		message = far.getMessage();
		return far.getResult();
	}
	
	public String getMessage()
	{
		return message;
	}
}

