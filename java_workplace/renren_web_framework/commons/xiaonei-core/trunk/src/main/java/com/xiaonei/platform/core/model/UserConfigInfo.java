package com.xiaonei.platform.core.model;

import java.io.Serializable;
import java.text.ParseException;

import org.json.JSONObject;

import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;
import com.xiaonei.platform.core.opt.permission.strategy.impl.PrivacyProfileUtil;

/**
 * 用户配置 UserConfig.java
 * 
 * @author fusongli
 */
public class UserConfigInfo extends mop.hi.oce.domain.user.UserConfigInfo implements Serializable {
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
	public static final long initEmailConfig = 262137;
	
	public static final int initBasicConfig = 3;
	
	public static final int initPokeConfig = 99;
	
	public static final int initRequestFriendConfig = 99;
	
	public static final int initBrowseConfig = 100;

	private static final long serialVersionUID = 4825282963196348511L;
	
	public UserConfigInfo() {
		super();
	}
	
	public int getOutsidePrivacyLevel(){
		int level = 0;
		if(browseConfig == 100){
			level = 5;
		}
		else{
			level = 0;
		}
		return level;
	}
	
	public int getProfilePrivacyLevel(){
		int level = 0;
		if(basicConfig == 1){
			level = 2;
		}
		else if(basicConfig == 3){
			level = 4;
		}
		else if(basicConfig == 99){
			level = 5;
		}
		return level;
	}
	
	public int getGoingPrivacyLevel(){
		int level = 0;
		int goingPrivacyValue = pokeConfig + requestFriendConfig;
		if(goingPrivacyValue == 198){
			level = 5;
		}
		else if(goingPrivacyValue < 198 && goingPrivacyValue > 6){
			level = 4;
		}
		else if(goingPrivacyValue <= 6 && goingPrivacyValue > 4){
			level = 3;
		}
		else if(goingPrivacyValue == 4){
			level = 2;
		}
		return level;
	}
	
	public int getPrivacyLevel(int value){
		int level = 0;
		switch(value){
			case 99: 
				level = 5;
				break;
			case 3:
				level = 4;
				break;
			case 1:
				level = 2;
				break;
			case -1:
				level = 1;
				break;
			case -2:
				level = 0;
				break;
		}
		return level;
	}
	
	/**
	 * 得到JSON对象
	 * @param ppString
	 * @return
	 */
	public static JSONObject getJSONObject(String ppString){
		//数据不全，做兼容处理
		if(ppString == null || ppString.trim().length() == 0) {
			ppString = PrivacyProfileUtil.ajaxJsonString();
		}
		JSONObject jsonObject = null;
		try {
			jsonObject = new JSONObject(ppString);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return jsonObject;		
	}
	
	private int getPpValue(String key) {
		int value = 99;
		if(null==key || key.length()==0)
			return value;
		
		String ppString = super.getProfilePrivacy();
		try {
			JSONObject jo = getJSONObject(ppString);
			if(null!=jo && null!=jo.getString(key)) {
				value = Integer.parseInt(jo.getString(key));
			}
		}catch(Exception e) {
			e.printStackTrace();
		}
		return value;
	}
	
	private boolean setPpValue(final String key, final int value, final int userid) {
		String ppString = super.getProfilePrivacy();
		try {
			JSONObject jo = getJSONObject(ppString);
			jo.put(key, value);
			this.setProfilePrivacy(jo.toString());
			WUserConfigInfoAdapter.getInstance().setUserConfig(userid, this);
		}catch(Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public int getAccountValue() {
		return this.getPpValue("a");
	}
	
	public boolean setAccountValue(final int value, final int userid) {
		return this.setPpValue("a", value, userid);
	}
	
	public int getBasicInfoValue() {
		return this.getPpValue("b");
	}
	
	public int getClassValue() {
		return this.getPpValue("c");
	}
	
	public boolean setGiftValue(final int value, final int userid) {
	    return this.setPpValue("d", value, userid);
	}
	
	public int getGiftValue() {
		return this.getPpValue("d");
	}
	
	public int getEducationValue() {
		return this.getPpValue("e");
	}
	
	public int getFriendValue() {
		return this.getPpValue("f");
	}
	
	public int getGossipValue() {
		return this.getPpValue("g");
	}
	
	public int getMobilelValue() {
		return this.getPpValue("h");
	}
	
	public int getFixedtelValue() {
		return this.getPpValue("i");
	}
	
	public int getBlogValue() {
		return this.getPpValue("j");
	}
	
	public int getAlbumValue() {
		return this.getPpValue("k");
	}
	
	public int getLinkValue() {
		return this.getPpValue("l");
	}
	
	public int getMsnValue() {
		return this.getPpValue("m");
	}
	
	public int getShareValue() {
		return this.getPpValue("n");
	}
	
	public int getPersonalValue() {
		return this.getPpValue("p");
	}
	
	public int getQqValue() {
		return this.getPpValue("q");
	}
	
	public int getStatusValue() {
		return this.getPpValue("s");
	}
	
	public int getTribeValue() {
		return this.getPpValue("t");
	}
	
	public int getFollowValue() {
		return this.getPpValue("v");
	}
	
	public boolean setFollow(final int value, final int userid) {
		return this.setPpValue("v", value, userid);
	}
	
	public int getWorkValue() {
		return this.getPpValue("w");
	}
	
	
	public int getEmailValue() {
		return this.getPpValue("o");
	}
	
	public boolean setEmail(final int value, final int userid) {
		return this.setPpValue("o", value, userid);
	}
	
	public int getAddressValue() {
		return this.getPpValue("r");
	}
	
	public boolean setAddress(final int value, final int userid) {
		return this.setPpValue("r", value, userid);
	}
	
	public int getRemarkValue() {
		return this.getPpValue("u");
	}
	
	public boolean setRemark(final int value, final int userid) {
		return this.setPpValue("u", value, userid);
	}
	
}
