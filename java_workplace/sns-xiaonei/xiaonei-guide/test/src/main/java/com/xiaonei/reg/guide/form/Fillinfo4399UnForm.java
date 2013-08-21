/**
 *
 */
package com.xiaonei.reg.guide.form;

import org.apache.struts.action.ActionForm;

/**
 * Fillinfo4399UnForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午08:52:11
 */
public class Fillinfo4399UnForm extends ActionForm {

    private static final long serialVersionUID = -4882685562466369699L;
    /**
     * 类型
     */
    private String typeOfCourse;
    /**
     * 大学
     */
    private String univName;
    /**
     * 大学
     */
    private int univId;

    public String getTypeOfCourse() {
        return typeOfCourse;
    }

    public void setTypeOfCourse(String typeOfCourse) {
        this.typeOfCourse = typeOfCourse;
    }

    public String getUnivName() {
        return univName;
    }

    public void setUnivName(String univName) {
        this.univName = univName;
    }

    public int getUnivId() {
        return univId;
    }

    public void setUnivId(int univId) {
        this.univId = univId;
	}

}
