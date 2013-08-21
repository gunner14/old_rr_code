package com.xiaonei.reg.register.pojo;

public class RegNetworkReginPojo extends RegNetworkBasePojo {
    private static final long serialVersionUID = 3960850850913071026L;
    private String email;
    private int homecitycode;
    private String homecityname;
    private String homeprovince;

    public RegNetworkReginPojo(int stage, int userid, String email, int homecitycode, String homecityname, String homeprovince) {
        super(stage, userid);
        this.setEmail(email);
        this.homecitycode = homecitycode;
        this.homecityname = homecityname;
        this.homeprovince = homeprovince;
    }

    /**
     * 地域网络 数据
     *
     * @param email
     * @param homecitycode
     * @param homecityname
     * @param homeprovince
     * @param stage
     * @param userid
     */

    public void setEmail(String email) {
        this.email = email;
    }

    public String getEmail() {
        return email;
    }

    public int getHomecitycode() {
        return homecitycode;
    }

    public void setHomecitycode(int homecitycode) {
        this.homecitycode = homecitycode;
    }

    public String getHomecityname() {
        return homecityname;
    }

    public void setHomecityname(String homecityname) {
        this.homecityname = homecityname;
    }

    public String getHomeprovince() {
        return homeprovince;
    }

    public void setHomeprovince(String homeprovince) {
        this.homeprovince = homeprovince;
	}

}
