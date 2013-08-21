package com.xiaonei.reg.register.model;

import com.xiaonei.reg.common.constants.IRegFromSource;

import java.io.Serializable;
import java.util.Date;

/**
 * RegFromSource 注册来源
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午03:17:34
 */
public class RegFromSource implements Serializable, IRegFromSource {

    private static final long serialVersionUID = -109301955810630900L;

    /**
     * user id int(20)
     */
    public Integer id;
    /**
     * 来源 varchar(20)
     */
    public String from;
    /**
     * 注册时间
     */
    public Date regTime;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getFrom() {
        return from;
    }

    public void setFrom(String from) {
        this.from = from;
    }

    public Date getRegTime() {
        return regTime;
    }

    public void setRegTime(Date regTime) {
        this.regTime = regTime;
	}

}
