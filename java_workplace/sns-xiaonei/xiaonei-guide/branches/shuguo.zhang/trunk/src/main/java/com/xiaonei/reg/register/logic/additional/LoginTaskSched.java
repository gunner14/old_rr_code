package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.sched.Schedulable;

public class LoginTaskSched implements Schedulable {


    public LoginTaskSched(User host) {
    }

    public String getDescription() {
        return null;
    }

    public long getInterval() {
        return 0;
    }

    public int getSchedTimes() {
        return 1;
    }

    public void run() {
    }
}
