/**
 *
 */
package com.xiaonei.reg.guide.pojo;

import java.io.Serializable;

/**
 * ProCity.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-6 上午11:42:57
 */
public class ProCity implements Serializable {

    private static final long serialVersionUID = 1359151175560550804L;

    private String province;
    private String city;

    public String toString() {
        return province + "   " + city;
    }

    /**
     * @param province
     * @param city
     */
    public ProCity(String province, String city) {
        this.province = province;
        this.city = city;
    }

    public String getProvince() {
        return province;
    }

    public String getCity() {
        return city;
    }

}
