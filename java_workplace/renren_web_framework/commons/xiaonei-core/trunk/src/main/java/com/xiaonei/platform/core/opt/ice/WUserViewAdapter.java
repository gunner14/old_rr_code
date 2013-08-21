package com.xiaonei.platform.core.opt.ice;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserHomeView;

/**
  *  只提供给home工程使用
 * @author zhangjie@opi-corp.com
 */
public class WUserViewAdapter {
	
	private volatile static WUserViewAdapter instance = null;
	
	private WUserViewAdapter(){
		
	}
	
	public static WUserViewAdapter getInstance(){
		if(instance==null){
			synchronized(WUserViewAdapter.class){
				if(instance==null){
					instance = new WUserViewAdapter();
				}
			}
		}
		return instance;
	}
	
	public UserHomeView getUserHomeView(int userId){
		UserHomeView homeView = new UserHomeView();
		
		User user = WUserAdapter.getInstance().get(userId);
		homeView.setUser(user);
		
		UserBasic userBasic = WUserBasicAdapter.getInstance().get(userId);
		homeView.setUserbasic(userBasic);
		
		UserDesc userDesc = WUserDescAdapter.getInstance().get(userId);
		homeView.setUserdesc(userDesc);
		
		return homeView;
	}
}
