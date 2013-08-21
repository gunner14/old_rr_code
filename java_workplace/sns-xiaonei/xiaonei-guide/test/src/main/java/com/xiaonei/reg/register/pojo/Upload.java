/**
 *
 */
package com.xiaonei.reg.register.pojo;

import java.io.Serializable;


/**
 * Upload
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-15 - 上午10:34:21
 */
public class Upload implements Serializable {
    private static final long serialVersionUID = 6057238188015614322L;
    private String large;
    private String main;
    private String head;
    private String tiny;

    public Upload(String large, String main, String head, String tiny) {
        super();
        this.large = large;
        this.main = main;
        this.head = head;
        this.tiny = tiny;
    }

    public String getLarge() {
        return large;
    }

    public String getMain() {
        return main;
    }

    public String getHead() {
        return head;
    }

    public String getTiny() {
        return tiny;
    }

}
