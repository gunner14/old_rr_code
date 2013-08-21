/**
 *
 */
package com.xiaonei.reg.guide.form;

import org.apache.struts.action.ActionForm;

/**
 * FillInfo4399BLForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午08:43:47
 */
public class FillInfo4399BLForm extends ActionForm {

    private static final long serialVersionUID = 5860804334281423198L;

    /**
     * 居住地
     */
    private String locationProvince;
    private String locationCity;
    private int locationCode;

    public String getLocationProvince() {
        return locationProvince;
    }

    public void setLocationProvince(String locationProvince) {
        this.locationProvince = locationProvince;
    }

    public String getLocationCity() {
        return locationCity;
    }

    public void setLocationCity(String locationCity) {
        this.locationCity = locationCity;
    }

    public int getLocationCode() {
        return locationCode;
    }

    public void setLocationCode(int locationCode) {
        this.locationCode = locationCode;
    }

}
