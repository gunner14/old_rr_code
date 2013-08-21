package com.xiaonei.platform.core.opt.ice;

import mop.hi.oce.adapter.impl.UserConfigInfoAdapterImpl;

import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.model.factory.WUserConfigInfoFactory;

public class WUserConfigInfoAdapter extends UserConfigInfoAdapterImpl {
	private static WUserConfigInfoAdapter instance = null;

	public static WUserConfigInfoAdapter getInstance() {
		if (instance == null) {
			synchronized (WUserConfigInfoAdapter.class) {
				if (instance == null)
					instance = new WUserConfigInfoAdapter(
							new WUserConfigInfoFactory());

			}
		}
		return instance;
	}

	protected WUserConfigInfoAdapter(WUserConfigInfoFactory fac) {
		super(fac);

	}

	public UserConfigInfo getUserConfig(int userId) {
		try {
			return (UserConfigInfo) super.getUserConfig(userId);
		} catch (Exception e) {
			System.err.println("-------userconfigInfo:" + userId);
			e.printStackTrace();
		}
		return null;
	}
	
	public void setUserConfig(int userid, UserConfigInfo uc)
	{
		try
		{
			super.setUserConfig(userid, uc);
		}catch(Exception e){e.printStackTrace();}
	}
}
