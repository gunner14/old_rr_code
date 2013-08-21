package com.xiaonei.sns.platform.core.opt.ice.impl;

import com.xiaonei.platform.core.model.UserContact;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserFav;
import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.platform.core.model.factory.WUserContactFactory;
import com.xiaonei.platform.core.model.factory.WUserDescFactory;
import com.xiaonei.platform.core.model.factory.WUserFavFactory;
import com.xiaonei.platform.core.model.factory.WUserTimeFactory;
import com.xiaonei.sns.platform.core.opt.ice.IUserDescAdapter;
import com.xiaonei.xce.usercontact.UserContactAdapter;
import com.xiaonei.xce.userdescview.UserDescViewAdapter;
import com.xiaonei.xce.userfav.UserFavAdapter;
import com.xiaonei.xce.usertime.UserTimeAdapter;
/**
 * @author zhangjie@opi-corp.com
 */
public class WUserDescAdapter extends BaseUsersAdapter<UserDescViewAdapter<UserDesc,UserTime,UserContact,UserFav>> implements IUserDescAdapter{
	private WUserTimeFactory timeFactory;
	private WUserContactFactory contactFactory;
	private WUserFavFactory favFactory;
	private UserTimeAdapter<UserTime> timeAdapter;
	private UserContactAdapter<UserContact> contactAdapter;
	private UserFavAdapter<UserFav> favAdapter;
	/**
	 * @deprecated
	 * @see {@linkplain SnsAdapterFactory#getUserDescAdapter()}
	 */
	public static IUserDescAdapter getInstance(){
		return SnsAdapterFactory.getUserDescAdapter();
	}
	
	@Override
	protected String getServiceName() {
	    return "userdesc";
	}
	@Override
	protected UserDescViewAdapter<UserDesc, UserTime, UserContact, UserFav> makeObject() {
	    return new UserDescViewAdapter<UserDesc,UserTime,UserContact,UserFav>(//
	            new WUserDescFactory(),//
	            new WUserTimeFactory(),//
	            new WUserContactFactory(),//
	            new WUserFavFactory()//
	            );
	}
	
	protected WUserDescAdapter(){
		timeFactory = new WUserTimeFactory();
		contactFactory = new WUserContactFactory();
		favFactory = new WUserFavFactory();
		
		timeAdapter = new UserTimeAdapter<UserTime>(timeFactory);
		contactAdapter = new UserContactAdapter<UserContact>(contactFactory);
		favAdapter = new UserFavAdapter<UserFav>(favFactory);
	}

    public UserDesc get(int id) {
        checkUserId(id);
        try {
            return getAdapter().getUserDescView(id);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
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
