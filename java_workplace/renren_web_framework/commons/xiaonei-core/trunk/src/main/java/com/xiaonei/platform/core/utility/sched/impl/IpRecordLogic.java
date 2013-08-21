package com.xiaonei.platform.core.utility.sched.impl;

import com.xiaonei.platform.core.utility.sched.SchedManager;

public class IpRecordLogic {

    public static void saveIpBlog(int userId, String ip, long eventId) {
        saveIpRecord(userId, ip, eventId, 0);
    }

    public static void saveIpGroup(int userId, String ip, int eventId) {
        saveIpRecord(userId, ip, eventId, 1);
    }

    public static void saveIpPhoto(int userId, String ip, long eventId) {
        saveIpRecord(userId, ip, eventId, 2);
    }

    private static void saveIpRecord(int userId, String ip, long eventId, int type) {
        try {
            IpRecordObject iro = new IpRecordObject();
            iro.setUserId(userId);
            iro.setIp(ip);
            iro.setType(type);
            iro.setEventId(eventId);
            IpRecordSched irs = new IpRecordSched(iro);
            SchedManager.getInstance().addTask(irs);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
