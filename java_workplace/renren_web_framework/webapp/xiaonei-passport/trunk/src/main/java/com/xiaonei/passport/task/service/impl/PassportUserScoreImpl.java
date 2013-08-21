package com.xiaonei.passport.task.service.impl;

import java.util.Calendar;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.task.service.PassportUserScore;
import com.xiaonei.wservice.userscore.login.LoginDays;
import com.xiaonei.wservice.userscore.login.client.LoginRecordAdapter;

@Service
public class PassportUserScoreImpl implements PassportUserScore {

	@Override
	public int getLoginConsecutiveCount(int userId) {
		LoginDays loginDays = LoginRecordAdapter.getInstance().getLoginDays(userId);
		if(null == loginDays){
		    return -1;
		}
        return loginDays
				.getContinueDays();
	}

	@Override
	public void incLoginCount(int userId, byte loginType) {
		LoginRecordAdapter.getInstance().incrLoginCount(userId, loginType);
	}

	@Override
	public boolean isTodayLogined(int userId) {
		Calendar c = Calendar.getInstance();
		c.set(Calendar.HOUR_OF_DAY, 0);
		c.set(Calendar.MINUTE, 0);
		c.set(Calendar.SECOND, 0);
		c.set(Calendar.MILLISECOND, 0);
		LoginDays loginDays = LoginRecordAdapter.getInstance().getLoginDays(userId);
		if(null == loginDays){
		    return false;
		}
        long lastTime = loginDays
				.getLastLoginTime();
		if (lastTime < c.getTimeInMillis()) {
			return false;
		} else {
			long timediff = Math.abs(System.currentTimeMillis() - lastTime);
			return timediff > 5 * 1000;
		}

	}

}
