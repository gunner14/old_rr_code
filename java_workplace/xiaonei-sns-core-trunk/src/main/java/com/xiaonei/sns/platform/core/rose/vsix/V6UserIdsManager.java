package com.xiaonei.sns.platform.core.rose.vsix;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

import org.springframework.beans.factory.InitializingBean;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserState;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.vsix.dao.VSixUserIdDAO;

public class V6UserIdsManager implements InitializingBean {

    public static List<Integer> v6UserIds = new ArrayList<Integer>();

    public static List<Integer> v6UserIdsTail = new ArrayList<Integer>();

    private volatile static boolean schduledRunning = false;

    private final static ReentrantLock lock = new ReentrantLock();

    private final static VSixUserIdDAO vSixUserIdDAO = VSixUserIdDAO.getInstance();

    private static final ScheduledExecutorService scheduled = Executors.newScheduledThreadPool(1);

    public static boolean isv6user(final User user) {
        scheduleCreateOnlyOneTime();
        if (user == null) {
            return false;
        }
        if (user.getUserState().isV6()) {
            return true;
        }
        final int userId = user.getId();
        final boolean containsId = v6UserIds.contains(userId);
        if (containsId) {
            return true;
        }
        for (final Integer userid : v6UserIdsTail) {
            if ((userId % 10) == userid) {
                return true;
            }
        }
        return false;
    }

    private static void loadUserIds() {
        final List<Integer> userIds = vSixUserIdDAO.getAllV6UserIds();
        final List<Integer> userIdsTail = new ArrayList<Integer>();
        for (final Integer userid : userIds) {
            if (userid < 10) {
                userIdsTail.add(userid);
            }
        }
        synchronized (v6UserIds) {
            v6UserIds = userIds;
        }
        synchronized (userIdsTail) {
            v6UserIdsTail = userIdsTail;
        }
    }

    private static void scheduleCreateOnlyOneTime() {
        if (schduledRunning) {
            return;
        }
        if (lock.isLocked()) {
            return;
        }
        lock.lock();
        try {
            if (schduledRunning) {
                return;
            }
            scheduled.scheduleWithFixedDelay(new Runnable() {

                @Override
                public void run() {
                    try {
                        V6UserIdsManager.loadUserIds();
                    } catch (final Throwable e) {
                        e.printStackTrace();
                    }

                }
            }, 0, 5, TimeUnit.MINUTES);
            schduledRunning = true;
        } finally {
            lock.unlock();
        }
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        V6UserIdsManager.scheduleCreateOnlyOneTime();
    }

    public boolean isV6User(final int userId) {
        final UserState userState = SnsAdapterFactory.getUserStateAdapter().getUserState(userId);
        if (userState == null) {
            return false;
        }
        if (userState.isV6()) {
            return true;
        }
        final boolean containsId = v6UserIds.contains(userId);
        if (containsId) {
            return true;
        }
        for (final Integer userid : v6UserIdsTail) {
            if ((userId % 10) == userid) {
                return true;
            }
        }
        return false;
    }

    public boolean isV6User(final User user) {
        if (user == null) {
            return false;
        }
        if (user.getUserState().isV6()) {
            return true;
        }
        final int userId = user.getId();
        final boolean containsId = v6UserIds.contains(userId);
        if (containsId) {
            return true;
        }
        for (final Integer userid : v6UserIdsTail) {
            if ((userId % 10) == userid) {
                return true;
            }
        }
        return false;
    }
}
