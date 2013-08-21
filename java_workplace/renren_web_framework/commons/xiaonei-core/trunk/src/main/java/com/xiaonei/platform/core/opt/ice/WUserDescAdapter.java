package com.xiaonei.platform.core.opt.ice;

import com.xiaonei.platform.core.model.UserContact;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserFav;
import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.platform.core.model.factory.WUserContactFactory;
import com.xiaonei.platform.core.model.factory.WUserDescFactory;
import com.xiaonei.platform.core.model.factory.WUserFavFactory;
import com.xiaonei.platform.core.model.factory.WUserTimeFactory;
import com.xiaonei.xce.usercontact.UserContactAdapter;
import com.xiaonei.xce.userdescview.UserDescViewAdapter;
import com.xiaonei.xce.userfav.UserFavAdapter;
import com.xiaonei.xce.usertime.UserTimeAdapter;
/**
 * @author zhangjie@opi-corp.com
 */
public class WUserDescAdapter{
	private WUserTimeFactory timeFactory;
	private WUserContactFactory contactFactory;
	private WUserFavFactory favFactory;
	private UserDescViewAdapter<UserDesc,UserTime,UserContact,UserFav> oceAdapter;
	private UserTimeAdapter<UserTime> timeAdapter;
	private UserContactAdapter<UserContact> contactAdapter;
	private UserFavAdapter<UserFav> favAdapter;
	private volatile static WUserDescAdapter instance = null;
	
	public static WUserDescAdapter getInstance(){
		if(instance==null){
			synchronized(WUserDescAdapter.class){
				if(instance==null){
					instance = new WUserDescAdapter();
				}
			}
		}
		return instance;
	}
	
	protected WUserDescAdapter(){
		timeFactory = new WUserTimeFactory();
		contactFactory = new WUserContactFactory();
		favFactory = new WUserFavFactory();
		
		oceAdapter = new UserDescViewAdapter<UserDesc,UserTime,UserContact,UserFav>(new WUserDescFactory(),timeFactory,contactFactory,favFactory);
		timeAdapter = new UserTimeAdapter<UserTime>(timeFactory);
		contactAdapter = new UserContactAdapter<UserContact>(contactFactory);
		favAdapter = new UserFavAdapter<UserFav>(favFactory);
	}
	
	public UserDesc get(int id) {
		if(id<=0)return null ;
		try{
			return (UserDesc)oceAdapter.getUserDescView(id);
		}catch(Exception e){
			e.printStackTrace();
		}
    return null ;		
	}
	
	public UserTimeAdapter<UserTime> getTimeAdapter(){
		return timeAdapter;
	}
	
	public UserContactAdapter<UserContact> getContactAdapter(){
		return contactAdapter;
	}
	
	public UserFavAdapter<UserFav> getFavAdapter(){
		return favAdapter;
	}
}
