package com.xiaonei.reg.register.pojo;

/**
 * 白领网络数据
 *
 * @author wang-tai
 */
public class RegNetworkMajorPojo extends RegNetworkBasePojo {

    private static final long serialVersionUID = 8339883116781447756L;

    private String email;
    private int netid;
    private String netname;
    private String workplacename;

    /**
     * 白领网络数据
     *
     * @param stage
     * @param userid
     * @param email
     * @param netid
     * @param netname
     * @param workplacename
     */
    public RegNetworkMajorPojo(int stage, int userid, String email, int netid, String netname, String workplacename) {
        super(stage, userid);
        this.email = email;
        this.netid = netid;
        this.workplacename = workplacename;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public int getNetid() {
        return netid;
    }

    public void setNetid(int netid) {
        this.netid = netid;
    }

    public String getNetname() {
        return netname;
    }

    public void setNetname(String netname) {
        this.netname = netname;
    }

    public String getWorkplacename() {
        return workplacename;
    }

    public void setWorkplacename(String workplacename) {
        this.workplacename = workplacename;
	}
}
