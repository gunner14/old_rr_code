package com.xiaonei.platform.core.opt.base;

import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;

/**
 * user_config表的want_see_css字段的管理器，<br/>
 * user_config表的 want_see_css 字段 目前是用于位操作的字段，目前的位的占用情况如下：<br/>
 * 第一位：用于表示是否看某人带有涂鸦样式的Profile页。<br/>
 * 第二位：用于表示用户是否从2007版Profile切换为2008版Profile页。<br/>
 * 第三位：用户表示一个用户是否安装过App，以区别于那些从来没有进行过app安装操作的用户。 <br/>
 * 第四位：首页新鲜事需要加入显示模式切换，新鲜事将会有简单view 和复合view 。 <br/>
 * 第五位：个人主页第一个Tab页中要放置一个第三方的AppBox，用这位表示是否有此AppBox，2008-12-15征用此位 <br/>
 * @author lifusong
 * 请维护
 */
public class UserConfigWantSeeCssMgr {
	/**
	 * 111111 
	 */
	private static int allBitIs_1 = 32+16+8+4+2+1; 
	/**
	 * 设置看其他人的美化页面，不屏蔽对方Profile涂鸦板,<br/>
	 * 操作user_configwant_see_css 的第一位，设置为 1 。
	 * 第一位 是 0:屏蔽。是 1:不屏蔽。
	 * @param uci 用户配置信息对象
	 */
	public static void setWantSeeCss(UserConfigInfo uci){
		try {
			//和1（二进制 1）做或操作
			int newValue = uci.getWantSeeCss() | 1;
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 设置不看其他人的美化页面，屏蔽对方Profile涂鸦板,<br/>
	 * 操作user_configwant_see_css 的第一位，设置为 0 。
	 * 第一位 是 0:屏蔽。是 1:不屏蔽。
	 * @param uci 用户配置信息对象
	 */
	public static void setNotWantSeeCss(UserConfigInfo uci){
		try {
			//和（二进制 111110）做 与 操作
			int newValue = uci.getWantSeeCss() & (allBitIs_1 - 1);
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/**
	 * 判断是否看其他人的美化页面，本方法是位操作，<br/>
	 * @param uci
	 * @return true 表示看对方美化过的页面， false 表示不看对方美化过的页面
	 */
	public static boolean isWantSeeCss(UserConfigInfo uci){
		try {
			//取出第一位的值，如果是 0 表示屏蔽，不想看对方美化过的页面， 如果是 1 表示不屏蔽，看对方美化过的页面
			return (uci.getWantSeeCss() & 1) == 1? true: false;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return false;
	}
	
	/**
	 * 切换到2008新版个人主页（宽版），本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第二位，设置为 1。<br/>
	 * 第二位 是 0:旧版。第二位 是 1:新版。
	 * @param uci 用户配置信息对象
	 */
	public static void switch2Profile2008(UserConfigInfo uci){
		try {
			//和2（二进制 10）做或操作
			int newValue = uci.getWantSeeCss() | 2;
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	

	/**
	 * 切换到2007旧版个人主页（窄版），本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第二位，设置为 0。<br/>
	 * 第二位 是 0:旧版。第二位 是 1:新版。
	 * @param uci 用户配置信息对象
	 */
	public static void switch2Profile2007(UserConfigInfo uci){
		try {
			//和（二进制 111101）做 与 操作
			int newValue = uci.getWantSeeCss() & (allBitIs_1 - 2);
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 判断是否切换到了Profile2008版，本方法是位操作，<br/>
	 * @param uci
	 * @return true 表示已经切换到2008版， false 表示没有切换到2008 ，还处于2007旧版
	 */
	public static boolean isSwitchProfile2008(UserConfigInfo uci){
		try {
			//取出第二位的值，如果是 0 表示旧版2007， 如果是 1 表示已经切换到新版2008
			return (uci.getWantSeeCss() & 2) == 2? true: false;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return false;
	}

	/**
	 * 设置用户已经安装过App，本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第三位，设置第三位为1。<br/>
	 * 第三位 是 0:没有安装过。第三位 是 1:安装过。
	 * @param uci 用户配置信息对象
	 */
	public static void setAlreadyInstalledApp(UserConfigInfo uci){
		try {
			//和4（二进制 100）做或操作
			int newValue = uci.getWantSeeCss() | 4;
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	/**
	 * 设置用户没有安装过App，本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第三位，设置第三位为0。<br/>
	 * 第三位 是 0:没有安装过。第三位 是 1:安装过。
	 * @param uci 用户配置信息对象
	 */
	public static void setNonInstallApp(UserConfigInfo uci){
		try {
			//和（二进制 111011）做 与 操作
			int newValue = uci.getWantSeeCss() & (allBitIs_1 - 4);
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/**
	 * 判断用户有没有安装过App，本方法是位操作，<br/>
	 * @param uci
	 * @return true 表示安装过， false 表示没有安装过
	 */
	public static boolean isInstalledApp(UserConfigInfo uci){
		try {
			//取出第三位的值，如果是 0 表示没有安装过， 如果是 1 表示安装过
			return (uci.getWantSeeCss() & 4) == 4? true: false;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return false;
	}
	
	

	/**
	 * 设置用户首页新鲜事以简单view展现，本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第四位，设置第四位为1。<br/>
	 * 第四位 是 0:默认显示 复合view 。第四位 是 1:简单view。
	 * @param uci 用户配置信息对象
	 */
	public static void setHomeFeedSimpleView(UserConfigInfo uci){
		try {
			//和8（二进制 1000）做或操作
			int newValue = uci.getWantSeeCss() | 8;
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	/**
	 * 设置用户首页新鲜事以复合view展现，本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第四位，设置第四位为0。<br/>
	 * 第四位 是 0:默认显示 复合view 。第四位 是 1:简单view。
	 * @param uci 用户配置信息对象
	 */
	public static void setHomeFeedComplexView(UserConfigInfo uci){
		try {
			//和（二进制 110111）做 与 操作
			int newValue = uci.getWantSeeCss() & (allBitIs_1 - 8);
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/**
	 * 判断用户首页的新鲜事展现是不是简单view，本方法是位操作，<br/>
	 * @param uci
	 * @return true 表示是简单view， false 表示是复合view
	 */
	public static boolean isHomeFeedSimpleView(UserConfigInfo uci){
		try {
			//取出第四位的值，如果是 0 表示简单view， 如果是 1 表示复合view
			return (uci.getWantSeeCss() & 8) == 8? true: false;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return false;
	}
	
	

	/**
	 * 设置个人主页第一个Tab页中有一个第三方的AppBox，本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第五位，设置第五位为1。<br/>
	 * 第五位 是 0:默认值 没有AppBox 。第五位 是 1:有AppBox。
	 * @param uci 用户配置信息对象
	 */
	public static void setProfileTabHasAppBox(UserConfigInfo uci){
		try {
			//和16（二进制 10000）做或操作
			int newValue = uci.getWantSeeCss() | 16;
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	/**
	 * 设置个人主页第一个Tab页中没有一个第三方的AppBox，本方法是位操作，<br/>
	 * 操作user_configwant_see_css 的第五位，设置第五位为0。<br/>
	 * 第五位 是 0:默认值 没有AppBox 。第五位 是 1:有AppBox。
	 * @param uci 用户配置信息对象
	 */
	public static void setProfileTabNoHasAppBox(UserConfigInfo uci){
		try {
			//和（二进制 101111）做 与 操作
			int newValue = uci.getWantSeeCss() & (allBitIs_1 - 16);
			save(uci.getId(),newValue);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/**
	 * 判断个人主页第一个Tab中是否有第三方的AppBox，本方法是位操作，<br/>
	 * @param uci
	 * @return true 表示有AppBox， false 表示没有AppBox 
	 */
	public static boolean isHasAppBoxOnProfileTab(UserConfigInfo uci){
		try {
			//取出第五位的值，如果是 0 表示没有AppBox， 如果是 1 表示有AppBox
			return (uci.getWantSeeCss() & 16) == 16? true: false;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return false;
	}
	
		
	
	/**
	 * 
	 * @param userId
	 * @param newValue
	 */
	private static void save(int userId, int newValue){
		try {
			UserConfigInfo u= new UserConfigInfo();
			u.setId(userId);
			u.setWantSeeCss(newValue);
//			XnLog.logger.info("newValue="+newValue);
//			XnLog.logger.info("userId="+userId);
			WUserConfigInfoAdapter.getInstance().setUserConfig(userId,u);
		} catch (Exception e) {
			// TODO: handle exception
		}
	}
}
