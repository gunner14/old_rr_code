package com.xiaonei.reg.guide.flows.xfive.logics;

import java.util.List;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

public class GuideXFiveUserFlagLogic {
	private static GuideXFiveUserFlagLogic instance = new GuideXFiveUserFlagLogic();

	public static GuideXFiveUserFlagLogic getInstance() {
		return instance;
	}

	public boolean isXFiveUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 5 
				|| host.getId() % 10 == 6
				) {
			ret = true;
		}
		return ret;
	}
	public boolean isXFiveAppMZMGUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		String fromsource = RegFromSourceLogic.getFromSource(host);
		List<String> list = RegFromSourceLogic.getFromSoureList(fromsource);
		if (list.contains(RegFromSourceLogic.FS_MAZIMIANGUAN)) {
			ret = true;
		}
		return ret;
	}
	
	public boolean isXFiveXUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 5 || host.getId() % 10 == 6) {
			ret = true;
		}
		return ret;
	}
	
	/**
	 * 应用于尾号7 8
	 * @param host
	 * @return
	 */
	public boolean isXFiveCoralUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 7 || host.getId() % 10 == 8) {
			ret = true;
		}
		return ret;
	}
	/**
	 * * 尾号：5 6 - 
	 * 应用于尾号5 6 2010年1月28日12:07:22
	 * 
	 * @param host
	 * @return
	 */
	public boolean isXFiveCoralBUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 5 || host.getId() % 10 == 6) {
			ret = true;
		}
		return ret;
	}
	public boolean isXFiveBlueUser(User host) {
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 7 || host.getId() % 10 == 8) {
			ret = true;
		}
		return ret;
	}
	
	public boolean isXFiveCrimsonUser(User host){
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 2 || host.getId() % 10 == 4) {
			ret = true;
		}
		return ret;
	}
	
	public boolean isXFiveChocoUser(User host){
		if (host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 0 || host.getId() % 10 == 1) {
			ret = true;
		}
		return ret;
	}
	
	/**
	 * * 尾号：0 1 2 4 - 
	 * 用于尾号0 1 2 4 的
	 * 加上5 6的 2010-1-7 14:42:18
	 * 去掉5 6的 2010-1-28 12:07:09 去掉4的
	 * @param host
	 * @return
	 */
	public boolean isXFiveCyanAUser(User host){
		if(host == null)
			return false;
		boolean ret = false;
		if (host.getId() % 10 == 0 
				|| host.getId() % 10 == 1
				|| host.getId() % 10 == 2
//				|| host.getId() % 10 == 4
//				|| host.getId() % 10 == 5
//				|| host.getId() % 10 == 6
		) {
			ret = true;
		}
		return ret;
	}
	
	/**
	 * 尾号：7 8 - 
	 * 用于尾号5 6 7 8 的
	 * 去掉5 6 的 2010-1-7 14:42:59
	 * 加上9 的 2010-1-11 16:08:35 诗伦
	 * @param host
	 * @return
	 */
	public boolean isXFiveCyanBUser(User host){
		if(host == null)
			return false;
		boolean ret = false;
		if (false
				|| host.getId() % 10 == 7
				|| host.getId() % 10 == 8
				) {
			ret = true;
		}
		return ret;
	}
	
	/**
	 * 尾号：0 1 2 3 4 5 6 7 8 9 - 
	 * 修改到应用于0到8所有尾号 2010-2-25 15:22:58
	 * 修改到0-9 2010年3月30日14:00:09
	 * @param host
	 * @return
	 */
	public boolean isXFiveBrownUser(User host){
		if(host == null)
			return false;
		boolean ret = false;
		if (false
//				|| host.getId() % 10 == 3
//				|| host.getId() % 10 == 4
//				|| host.getId() % 10 != 9
				|| true
				) {
			ret = true;
		}
		return ret;
	}
	
	public boolean isXFiveBrown0User(User host) {
		if(host == null)
			return false;
		boolean ret = false;
		if(false
				|| host.getId() % 10 == 0
				){
			return true;
		}
		return ret;
	}
	
	/**
	 * 尾号： 0 1 2 3 4 5 6 7 8 9-
	 * 2010-5-18 10:41:35 创建
	 * 2010-5-26 15:44:47 诗伦加上5-9
	 * @param host
	 * @return
	 */
	public boolean isXFiveDarkBlueUser(User host) {
		if(host == null)
			return false;
		boolean ret = false;
		if(false
				|| host.getId() % 10 == 0
				|| host.getId() % 10 == 1
				|| host.getId() % 10 == 2
				|| host.getId() % 10 == 3
				|| host.getId() % 10 == 4
				|| host.getId() % 10 == 5
				|| host.getId() % 10 == 6
				|| host.getId() % 10 == 7
				|| host.getId() % 10 == 8
				|| host.getId() % 10 == 9
				){
			return true;
		}
		return ret;
	}
	
	/**
	 * 尾号 3 4 5 6 7 8 9 0 1 2-
	 * 陈周的需求 2010-3-8 10:45:59
	 * 陈周的需求 2010-3-10 19:00:56 加上其它尾号
	 * 陈周的需求 2010-4-21 16:46:57 去掉0到4的尾号
	 * @param host
	 * @return
	 */
	public boolean isSKDefault(User host){
		if(host == null)
			return false;
		String domain = host.getDomain();
		if(MyStringUtils.contains(domain, "renren")) 
			return false;
		boolean ret = false;
//		if(false
//				|| host.getId() % 10 == 5
//				|| host.getId() % 10 == 6
//				|| host.getId() % 10 == 7
//				|| host.getId() % 10 == 8
//				|| host.getId() % 10 == 9
//				|| host.getId() % 10 == 0
//				|| host.getId() % 10 == 1
//				|| host.getId() % 10 == 2
//				|| host.getId() % 10 == 3
//				|| host.getId() % 10 == 4
//				){
//			return true;
//		}
		return ret;
	}



}
