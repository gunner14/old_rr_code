package com.xiaonei.platform.component.gossip.biz;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.platform.component.gossip.model.GossipRight;
import com.xiaonei.platform.component.space.home.SpaceHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.adminPermission.AdminManager;
import com.xiaonei.platform.core.opt.permission.License;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.opt.permission.strategy.impl.ProfilePrivacy;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Service
public class GossipRightBiz {
	
	private Log logger = LogFactory.getLog(this.getClass());
	
	public boolean hasGossipRight(User host,User guest,GossipRight right){
		UserConfigInfo userUci = null;
		try {
			userUci = SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(guest.getId());
		} catch (Exception e) {
			return false;
		}

		//
		License license = null;// 许可对象，是否可以访问主页的许可
		int path = RelationDef.uuNoPath;// 计算path, uuFriend
		try {
			// 登录情况下，可以判断 2 人关系 path
			int sourceControl = 0;// 被访问者的基本设置
			if (userUci != null) {
				sourceControl = userUci.getBasicConfig();
			}
			// 采用新的封装
			license = SpaceHome.getInstance().getVisitPermission(host, guest, sourceControl);
			path = license.getPath();
		} catch (Exception e) {
			return false;
		}
		// 访问者的auth值（有些访问者是管理员）页面上的单个项的隐私控制
		int authValue = host.getAuth();
		ProfilePrivacy pp = null;
		if (userUci != null) {
			if ((userUci.getProfilePrivacy() == null)
			|| (userUci.getProfilePrivacy().trim().length() == 0)) {
				pp = new ProfilePrivacy(path, authValue);
			} else {
				pp = new ProfilePrivacy(path, userUci.getProfilePrivacy(), authValue);
			}
		}
		switch(right){
		case ShowBoard:return pp.getShowGossipBoard();
		case Post: return pp.getShowGossipBoard();
		case Get: return pp.getShowGossip();
		default : return false;
		}
	}
	
	public boolean hasRight(int host,int f,int r){
		try {
			return  AdminManager.getInstance().hasRight(host, f, r);
		} catch (Exception e) {
			logger.warn("管理员判断权限异常：",e);
			return false;
		}
	}
	

}
