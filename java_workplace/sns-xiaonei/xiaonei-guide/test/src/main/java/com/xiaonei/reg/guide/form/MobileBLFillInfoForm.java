/**
 *
 */
package com.xiaonei.reg.guide.form;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;
import org.apache.struts.action.ActionForm;

import java.io.Serializable;

/**
 * MobileBLFillInfoForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午06:18:03
 */
public class MobileBLFillInfoForm extends ActionForm implements Serializable {

    private static final long serialVersionUID = -8259466567759297442L;

    /**
     * 性别
     */
    private String gender;
    /**
     * 居住地
     */
    private String locationProvince;
    private String locationCity;
    private int locationCode;
    /**
     * 公司
     */
    private String company;
    /**
     * 阶段
     */
    private int stage;

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
    	gender = com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent(gender);
        this.gender = gender;
    }

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public String getLocationProvince() {
        return locationProvince;
    }

    public void setLocationProvince(String locationProvince) {
    	locationProvince = com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent(locationProvince);
        this.locationProvince = locationProvince;
    }

    public String getLocationCity() {
        return locationCity;
    }

    public void setLocationCity(String locationCity) {
    	locationCity = com.xiaonei.websecurity.adapter.SafeFilter.getInstance().getPureContent(locationCity);
        this.locationCity = locationCity;
    }

    public int getLocationCode() {
        return locationCode;
    }

    public void setLocationCode(int locationCode) {
        this.locationCode = locationCode;
    }

    /**
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE).appendSuper(super.toString()).append("locationCode", this.locationCode).append(
                "locationCity", this.locationCity).append("multipartRequestHandler", this.multipartRequestHandler).append("company", this.company).append(
                "gender", this.gender).append("stage", this.stage).append("locationProvince", this.locationProvince).append("servlet", this.servlet).toString();
	}

}
