package com.xiaonei.reg.register.logic.additional;

import mop.hi.oce.domain.feed.FeedFilter;

import com.xiaonei.platform.component.application.facade.xiaonei_reg.AppUserFacade;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.register.constants.IBasicControlDef;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;

/**
 * 负责设置用户信息配置 UserConfig
 * 
 * @author wang-tai
 */
public class RegUserConfigLogic {
	private static RegUserConfigLogic _instance = new RegUserConfigLogic();

	public static RegUserConfigLogic getInstance() {
		return _instance;
	}

	private RegUserConfigLogic() {
		super();
	}

	/**
	 * 配置用户信息
	 * 
	 * @param oceUser
	 *            用户
	 */
	public void configUserInfo(RegUserConfigPojo uc) {
	    if(uc!=null && uc.getUser()!=null){
    		switch (uc.getUser().getStage()) {
    		case Stage.STAGE_HEIGHSCHOOL:
    			this.setHsMenu(uc);
    			break;
    		case Stage.STAGE_UNIV:
    			this.setUnivMenu(uc);
    			break;
    		case Stage.STAGE_MAJOR:
    			this.setMajorMenu(uc);
    			break;
    		case Stage.STAGE_NONE:
    			this.setMajorMenu(uc);
    			break;
    		default:
    			this.setMajorMenu(uc);
    			break;
    		}
	    }
	}

	public void setHsMenu(RegUserConfigPojo uc) {
		AppUserFacade.getInstance().installDefaultApps(uc.getUser(),
				"kaixin.com".equals(uc.getUser().getDomain()));
	}

	public void setUnivMenu(RegUserConfigPojo uc) {
		AppUserFacade.getInstance().installDefaultApps(uc.getUser(),
				"kaixin.com".equals(uc.getUser().getDomain()));
	}

	public void setMajorMenu(RegUserConfigPojo uc) {
		AppUserFacade.getInstance().installDefaultApps(uc.getUser(),
				"kaixin.com".equals(uc.getUser().getDomain()));
	}

	/**
	 * 设置隐私
	 * 
	 * @param uc
	 * @param config
	 */
	private void setPrivacy(int userId, UserConfigInfo config) {
		UserConfigInfo uci = config;
		if (null == config)
			return;
		try {
			int basicControl = IBasicControlDef.ALL_NETWORK;
			uci.setBasicConfig(basicControl);
			uci.setPokeConfig(basicControl);
			//2010-02-22修改默认的设置
			uci
					.setProfilePrivacy("{a:3,c:0,e:3,f:0,g:3,j:0,k:0,0:0,m:0,p:3,q:0,s:0,t:0,w:3,h:0,n:0,d:0,i:0}");
			uci.setBrowseConfig(99);
			WUserConfigInfoAdapter.getInstance().createUserConfig(userId, uci);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void doConfig(final int userId) {
		try {
			UserConfigInfo config = new UserConfigInfo();
			config.setId(userId);
			config.setEmailConfig(UserConfigInfo.initEmailConfig);
			config.setBasicConfig(UserConfigInfo.initBasicConfig);
			config.setPokeConfig(UserConfigInfo.initPokeConfig);
			config
					.setRequestFriendConfig(UserConfigInfo.initRequestFriendConfig);
			config.setBrowseConfig(UserConfigInfo.initBrowseConfig);
			config.setRecvFeedConfig(FeedFilter.FEED_FILTER_DEFAULT);

			// 将user_config表的want_see_css的第二位置成1（位操作），
			// 表示走新版的Profile页面
			int newProfile = 2;
			config.setWantSeeCss(newProfile);
			this.setPrivacy(userId, config);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
