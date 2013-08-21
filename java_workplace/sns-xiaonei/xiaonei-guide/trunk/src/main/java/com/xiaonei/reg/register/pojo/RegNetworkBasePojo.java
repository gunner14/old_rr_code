package com.xiaonei.reg.register.pojo;

import java.io.Serializable;

/**
 * 注册时网络信息
 *
 * @author wang-tai
 */
public class RegNetworkBasePojo implements Serializable {
    private static final long serialVersionUID = -6352137749262315450L;

    private int userid;
    private int stage;

    public RegNetworkBasePojo(int stage, int userid) {
        this.stage = stage;
        this.userid = userid;
    }

    public int getUserid() {
        return userid;
    }

    public int getStage() {
        return stage;
    }

}
