package com.xiaonei.reg.register.model;

import java.io.Serializable;


/**
 * ViewCount.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-23 下午08:00:34
 */
public class ViewCount implements Serializable {
    private static final long serialVersionUID = 8234650290181069541L;
    private int id;
    private int viewcount;

    public ViewCount(int usr) {
        super();
        this.id = usr;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getViewcount() {
        return viewcount;
    }

    public void setViewcount(int viewcount) {
        this.viewcount = viewcount;
    }
}
