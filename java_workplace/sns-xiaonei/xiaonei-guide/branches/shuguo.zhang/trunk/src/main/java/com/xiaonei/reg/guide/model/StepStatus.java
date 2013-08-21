package com.xiaonei.reg.guide.model;

import java.io.Serializable;

public class StepStatus implements Serializable {

    private static final long serialVersionUID = -8780957541421256459L;
    private int id;
    private int status;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }


}
