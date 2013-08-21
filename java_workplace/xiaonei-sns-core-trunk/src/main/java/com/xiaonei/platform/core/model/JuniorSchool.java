package com.xiaonei.platform.core.model;

import java.io.Serializable;

public class JuniorSchool extends Network implements Serializable {

    private static final long serialVersionUID = 4001252591245754735L;

    private int flag;

    /** 所在区县编码 */
    private String code;

    /** 所在省编码 */
    private String code_pro;

    /** 所在城市编码 */
    private String code_city;

    /** 所在区县编码 */
    private String code_county;

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }

    public String getCode_city() {
        return code_city;
    }

    public void setCode_city(String code_city) {
        this.code_city = code_city;
    }

    public String getCode_county() {
        return code_county;
    }

    public void setCode_county(String code_county) {
        this.code_county = code_county;
    }

    public String getCode_pro() {
        return code_pro;
    }

    public void setCode_pro(String code_pro) {
        this.code_pro = code_pro;
    }

}
