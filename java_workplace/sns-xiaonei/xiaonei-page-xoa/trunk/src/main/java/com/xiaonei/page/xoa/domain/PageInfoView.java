package com.xiaonei.page.xoa.domain;

import java.util.Date;

public class PageInfoView{

    private Integer pageId;

    /** 基本信息数据 */
    private String basic;

    /** 详细信息 数据 */
    private String detail;

    /** 联系信息数据 */
    private String contact;

    /** 添加时间 */
    private Date addTime;

    /** 更新时间 */
    private Date updateTime;
    
    /** page 类型*/
    private Integer ptype;

	public Integer getPageId() {
		return pageId;
	}

	public void setPageId(Integer pageId) {
		this.pageId = pageId;
	}

	public String getBasic() {
		return basic;
	}

	public void setBasic(String basic) {
		this.basic = basic;
	}

	public String getDetail() {
		return detail;
	}

	public void setDetail(String detail) {
		this.detail = detail;
	}

	public String getContact() {
		return contact;
	}

	public void setContact(String contact) {
		this.contact = contact;
	}

	public Date getAddTime() {
		return addTime;
	}

	public void setAddTime(Date addTime) {
		this.addTime = addTime;
	}

	public Date getUpdateTime() {
		return updateTime;
	}

	public void setUpdateTime(Date updateTime) {
		this.updateTime = updateTime;
	}

	public Integer getPtype() {
		return ptype;
	}

	public void setPtype(Integer ptype) {
		this.ptype = ptype;
	}
    
    

}
