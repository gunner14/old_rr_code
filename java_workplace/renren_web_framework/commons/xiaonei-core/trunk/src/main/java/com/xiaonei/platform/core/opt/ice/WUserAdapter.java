package com.xiaonei.platform.core.opt.ice;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserName;
import com.xiaonei.platform.core.model.UserPassport;
import com.xiaonei.platform.core.model.UserRight;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.platform.core.model.UserUrl;
import com.xiaonei.platform.core.model.factory.WUserCommonFactory;
import com.xiaonei.platform.core.model.factory.WUserNameFactory;
import com.xiaonei.platform.core.model.factory.WUserPassportFactory;
import com.xiaonei.platform.core.model.factory.WUserRightFactory;
import com.xiaonei.platform.core.model.factory.WUserStateFactory;
import com.xiaonei.platform.core.model.factory.WUserUrlFactory;
import com.xiaonei.xce.userbaseview.UserBaseCommonViewAdapter;
import com.xiaonei.xce.userlogincount.UserLoginCountAdapter;
import com.xiaonei.xce.username.UserNameAdapter;
import com.xiaonei.xce.userpassport.NoSuchUserByEmailException;
import com.xiaonei.xce.userpassport.UserPassportAdapter;
import com.xiaonei.xce.userright.UserRightAdapter;
import com.xiaonei.xce.userstate.UserStateAdapter;
import com.xiaonei.xce.userurl.UserUrlAdapter;
/**
 * @author zhangjie@opi-corp.com
 * @version 0.1 对ice cache user对象的封装 和数据库users表对应，users 数据操作的入口
 */
public class WUserAdapter{
	private WUserPassportFactory passportFactory;
	private WUserRightFactory rightFactory;
	private WUserNameFactory nameFactory;
	private WUserStateFactory stateFactory;
	private WUserUrlFactory urlFactory;
	
	private UserBaseCommonViewAdapter<User,UserPassport,UserRight,UserName,UserState,UserUrl> oceAdapter;
	private UserLoginCountAdapter loginCountAdapter;
	private UserPassportAdapter<UserPassport> passportAdapter; 
	private UserRightAdapter<UserRight> rightAdapter;
	private UserNameAdapter<UserName> nameAdapter;
	private UserStateAdapter<UserState> stateAdapter;
	private UserUrlAdapter<UserUrl> urlAdapter;
	
	private volatile static WUserAdapter instance = null;
	
	private WUserAdapter(){
		passportFactory = new WUserPassportFactory();
		rightFactory = new WUserRightFactory();
		nameFactory = new WUserNameFactory();
		stateFactory = new WUserStateFactory();
		urlFactory = new WUserUrlFactory();
		
		oceAdapter = new UserBaseCommonViewAdapter<User,UserPassport,UserRight,UserName,UserState,UserUrl>(new WUserCommonFactory(),passportFactory,rightFactory,nameFactory,stateFactory,urlFactory);
		
		loginCountAdapter = new UserLoginCountAdapter();
		passportAdapter = new UserPassportAdapter<UserPassport>(passportFactory);
		rightAdapter = new UserRightAdapter<UserRight>(rightFactory);
		nameAdapter = new UserNameAdapter<UserName>(nameFactory);
		stateAdapter = new UserStateAdapter<UserState>(stateFactory);
		urlAdapter = new UserUrlAdapter<UserUrl>(urlFactory);
	}
	
	public static WUserAdapter getInstance() {
		if(instance==null){
			synchronized(WUserAdapter.class){
				if(instance==null){
					instance = new WUserAdapter();
				}
			}
		}
		return instance;
	}
	
	public User get(int id) {
		if (id <= 0)return null;
		return (User)oceAdapter.getUserBaseView(id);
	}
	/**
	 * 根据email获取用户信息
	 * @param email
	 * @return 如果用户不存在返回null
	 */
	public User getByEmail(String email){
		int userId;
		try {
			userId = passportAdapter.getUserIdByEmail(email);
		} catch (NoSuchUserByEmailException e) {
			return null;
		}
		return get(userId);
	}
	
	public UserLoginCountAdapter getLoginCountAdapter(){
		return loginCountAdapter;
	}
	
	public UserPassportAdapter<UserPassport> getPassportAdapter(){
		return passportAdapter;
	}
	
	public UserRightAdapter<UserRight> getRightAdapter(){
		return rightAdapter;
	}
	
	public UserNameAdapter<UserName> getNameAdapter(){
		return nameAdapter;
	}
	
	public UserStateAdapter<UserState> getStateAdapter(){
		return stateAdapter;
	}
	
	public UserUrlAdapter<UserUrl> getUrlAdapter(){
		return urlAdapter;
	}
}
