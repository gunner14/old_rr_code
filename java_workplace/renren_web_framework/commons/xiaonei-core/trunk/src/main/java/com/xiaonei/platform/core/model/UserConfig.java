package com.xiaonei.platform.core.model;

import java.io.Serializable;

/**
 * 用户配置 UserConfig.java
 * 
 * @author fusongli
 */
public class UserConfig implements Serializable {
	/**
	 * 邮件提醒设置分类，有新的类型，请按2的指数次幂增加<br>
	 * value="1" 申请加我为好友<br>
	 * value="2" 邀请我加入群/圈子<br>
	 * value="4" 在我的照片中圈人或者在别人的照片中圈出我<br>
	 * value="8" 有新的留言和回复提示<br>
	 * value="16" 有人跟我打招呼了<br>
	 * value="32" 朋友最近三天的新鲜事<br>
	 * value="64" 我加入的群/圈子最近三天的新主题<br>
	 * value="128" 我的学校最近三天的新鲜事<br>
	 * value="256" 朋友的生日提醒<br>
	 * value="512" 站内活动<br>
	 * value="1024" 加朋友成功<br>
	 */
	/**email 配置，初始化为 25L */
	public static final long initEmailConfig = 25L;
	
	public static final int initBasicConfig = 40;
	
	public static final int initPokeConfig = 99;
	
	public static final int initRequestFriendConfig = 99;
	
	public static final int initBrowseConfig = 100;

	private static final long serialVersionUID = 4825282963196348511L;

	
	public UserConfig() {
	}

	

}
