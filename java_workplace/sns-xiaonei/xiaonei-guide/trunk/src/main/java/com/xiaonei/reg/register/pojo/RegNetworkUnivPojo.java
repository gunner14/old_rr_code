package com.xiaonei.reg.register.pojo;

/**
 * 大学网络数据
 *
 * @author wang-tai
 */
public class RegNetworkUnivPojo extends RegNetworkBasePojo {

    private static final long serialVersionUID = -1439906625908561195L;
    private String degree;
    private String univsityName;
    private int univsityid;

    /**
     * 大学网路 数据
     *
     * @param stage
     * @param userid
     * @param degree
     * @param univsityName
     * @param univsityid
     */
    public RegNetworkUnivPojo(int stage, int userid, String degree, String univsityName, int univsityid) {
        super(stage, userid);
        this.degree = degree;
        this.univsityName = univsityName;
        this.univsityid = univsityid;
    }

    public String getDegree() {
        return degree;
    }

    public String getUnivsityName() {
        return univsityName;
    }

    public int getUnivsityid() {
        return univsityid;
	}
}
