package com.xiaonei.passport.task.task;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.dbwrite.AccountServiceFactory;
import com.xiaonei.passport.task.followmission.IFollowMissionTask;

public class LoginTimeTask implements IFollowMissionTask{

	protected Log logger = LogFactory.getLog(this.getClass());
	
	private int userId ; 
	private String userAccount;
	public LoginTimeTask(int userId, String userAccount){
		this.userId = userId;
		this.userAccount = userAccount;
	}
	
	@Override
	public void run() {
		boolean result = AccountServiceFactory.getInstance().updateUserAccountsLastLoginTime(userId, userAccount);
		if(! result){
			logger.info("updateUserAccountsLastLoginTime userId:" + userId +", account:" + userAccount +"result: "+ result);
		}
	}

	@Override
	public String getName() {
		return "LoginTimeTask";
	}

}
