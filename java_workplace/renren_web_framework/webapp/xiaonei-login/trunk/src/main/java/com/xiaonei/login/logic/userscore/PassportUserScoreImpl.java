package com.xiaonei.login.logic.userscore;

import java.util.Calendar;

import org.springframework.stereotype.Service;

import com.xiaonei.wservice.userscore.login.LoginDays;
import com.xiaonei.wservice.userscore.login.client.LoginRecordAdapter;

@Service
public class PassportUserScoreImpl implements PassportUserScore {

    @Override
    public int getLoginConsecutiveCount(final int userId) {
        final LoginDays loginDays = LoginRecordAdapter.getInstance().getLoginDays(userId);
        if(null == loginDays){
            return -1;
        }
        return loginDays
                .getContinueDays();
    }

    @Override
    public void incLoginCount(final int userId, final byte loginType) {
        LoginRecordAdapter.getInstance().incrLoginCount(userId, loginType);
    }

    @Override
    public boolean isTodayLogined(final int userId) {
        final Calendar c = Calendar.getInstance();
        c.set(Calendar.HOUR_OF_DAY, 0);
        c.set(Calendar.MINUTE, 0);
        c.set(Calendar.SECOND, 0);
        c.set(Calendar.MILLISECOND, 0);
        final LoginDays loginDays = LoginRecordAdapter.getInstance().getLoginDays(userId);
        if(null == loginDays){
            return false;
        }
        final long lastTime = loginDays
                .getLastLoginTime();
        if (lastTime < c.getTimeInMillis()) {
            return false;
        } else {
            final long timediff = Math.abs(System.currentTimeMillis() - lastTime);
            return timediff > (5 * 1000);
        }

    }

}
