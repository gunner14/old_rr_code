package com.renren.renrengame.utils;

public class Security {

	public boolean varifyTicket(String ticket, String userId)
	{
		mop.hi.oce.adapter.impl.PassportAdapterImpl passport = new mop.hi.oce.adapter.impl.PassportAdapterImpl();
		Integer retId = passport.verifyTicket(ticket);
		System.out.println("Security.varifyTicket:"+retId.intValue()+"   userId:"+userId);
		if (retId.intValue() == -1)
			return false;
	//	com.xiaonei.xce.passport.PassportAdapterFactory.getWebTicket();
		
		if (retId.intValue() != Integer.parseInt(userId))
			return false;
		
		return true;
	}
}
