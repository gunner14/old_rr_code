/**
 *
 */
package com.xiaonei.reg.register.pojo;

/**
 * RegNetworkHSPojo.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-28 下午04:21:34
 */
public class RegNetworkHSPojo extends RegNetworkBasePojo {

    private static final long serialVersionUID = 2345777510705548443L;

    private String highSchoolName;

    private int infoId;
    private int juniorHighSchoolId;

    public RegNetworkHSPojo(int stage, int userid, int infoId, int juniorHighSchoolId, String highSchoolName) {
        super(stage, userid);
        this.infoId = infoId;
        this.juniorHighSchoolId = juniorHighSchoolId;
        this.highSchoolName = highSchoolName;
    }

    public String getHighSchoolName() {
        return highSchoolName;
    }

    public int getInfoId() {
        return infoId;
    }

    public int getJuniorHighSchoolId() {
        return juniorHighSchoolId;
    }

    public void setHighSchoolName(String highSchoolName) {
        this.highSchoolName = highSchoolName;
    }

    public void setInfoId(int infoId) {
        this.infoId = infoId;
    }

    public void setJuniorHighSchoolId(int juniorHighSchoolId) {
        this.juniorHighSchoolId = juniorHighSchoolId;
    }
}
