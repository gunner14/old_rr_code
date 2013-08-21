package com.xiaonei.reg.guide.form;

import org.apache.struts.action.ActionForm;

import java.io.Serializable;

public class ShortFormFillInfoBaseForm extends ActionForm implements Serializable {

    private static final long serialVersionUID = -637762490702991738L;

    /**
     * 用户类型
     */
    private int stage;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }
	
}
