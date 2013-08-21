package com.xiaonei.reg.guide.pojo;

import com.xiaonei.platform.core.model.User;

/**
 * @author dream
 * @date 2008-10-15
 */
public class HSFillInfoJuniorPojo {

    private int juniorHighSchoolId;

    private String juniorHighSchoolName;

    private int juniorHighSchoolYear;

    private User host;

    public int getJuniorHighSchoolId() {
        return juniorHighSchoolId;
    }

    public void setJuniorHighSchoolId(int juniorHighSchoolId) {
        this.juniorHighSchoolId = juniorHighSchoolId;
    }

    public String getJuniorHighSchoolName() {
        return juniorHighSchoolName;
    }

    public void setJuniorHighSchoolName(String juniorHighSchoolName) {
        this.juniorHighSchoolName = juniorHighSchoolName;
    }

    public int getJuniorHighSchoolYear() {
        return juniorHighSchoolYear;
    }

    public void setJuniorHighSchoolYear(int juniorHighSchoolYear) {
        this.juniorHighSchoolYear = juniorHighSchoolYear;
    }

    public User getHost() {
        return host;
    }

    public void setHost(User host) {
        this.host = host;
    }

}
