/**
 *
 */
package com.xiaonei.reg.guide.model;

import java.io.Serializable;

/**
 * ChinaArea.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-5 下午05:37:04
 */
public class ChinaArea implements Serializable {

    private static final long serialVersionUID = -2069029617328448701L;

    private String code;
    private String name;

    /**
     * @param code
     * @param name
     */
    public ChinaArea(String code, String name) {
        this.code = code;
        this.name = name;
    }

    public String getCode() {
        return code;
    }

    public String getName() {
        return name;
    }
}
