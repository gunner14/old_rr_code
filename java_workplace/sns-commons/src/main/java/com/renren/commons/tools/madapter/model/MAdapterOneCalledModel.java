package com.renren.commons.tools.madapter.model;

import java.io.Serializable;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Mar 27, 2012 3:21:08 PM
 * @Who 包含方法执行各种信息的一个model
 * @Modify
 */
public class MAdapterOneCalledModel implements Serializable {

    private static final long serialVersionUID = 574710669509278689L;

    private boolean success;

    private long calledTime;

    private long usedMemory;

    public boolean isSuccess() {
        return success;
    }

    public void setSuccess(final boolean success) {
        this.success = success;
    }

    public long getCalledTime() {
        return calledTime;
    }

    public void setCalledTime(final long calledTime) {
        this.calledTime = calledTime;
    }

    public long getUsedMemory() {
        return usedMemory;
    }

    public void setUsedMemory(final long usedMemory) {
        this.usedMemory = usedMemory;
    }

}
