package com.xiaonei.platform.core.model;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Date;

import com.xiaonei.xce.userdescview.UserDescView;

/**
 * @author zhangjie@opi-corp.com
 */
public class UserDesc extends UserDescView<UserTime,UserContact,UserFav> implements Serializable {
	private static final long serialVersionUID = -2573032935427223078L;
	
	public static final int flagNotUpdate = 3;
	public static final int flagUpdated = 7;
	public static final int userbanned = 1;
	public static final int deuserban = 0;

	//UserTime Begin
	public Date getActivateTime() {
		return this.userTimeView.getActivateTime();
	}
	
	public Date getLastLoginTime() {
		return this.userTimeView.getLastLoginTime();
	}
	
	public Date getLastLogoutTime() {
		return this.userTimeView.getLastLogoutTime();
	}
	
	public Date getRegisterTime() {
		return this.userTimeView.getRegisterTime();
	}
	//UserTime End
	
	//UserContact Begin
	public String getFixPhone() {
		return this.userContactView.getFixPhone();
	}
	
	public String getHomepage() {
		return this.userContactView.getHomepage();
	}
	
	public String getMobilePhone() {
		return this.userContactView.getMobilePhone();
	}
	
	public String getMsn() {
		return this.userContactView.getMsn();
	}
	
	public long getQq() {
		return this.userContactView.getQq();
	}
	//UserContact End
	
//	by zhaoning
//	//UserFav Begin
//	public String getFav() {
//		return this.userFavView.getFav();
//	}
//	public int getFavFlag() {
//		return this.userFavView.getFavFlag();
//	}
	//UserFav End

	//Old UserDesc Remain Begin
	public int getNoLoginDays(){
		try {
			Calendar lc = Calendar.getInstance();
			lc.setTime(this.userTimeView.getLastLoginTime());
			int lastDay = lc.get(Calendar.DAY_OF_YEAR);
			
			Calendar cc = Calendar.getInstance();
			cc.setTime(new Date());
			int currDay = cc.get(Calendar.DAY_OF_YEAR);
			
			int days =  currDay - lastDay ;
			return days;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return 0;
	}
	//Old UserDesc Remain End
}