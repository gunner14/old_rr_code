package com.xiaonei.reg.guide.pojo;

import java.io.Serializable;



/**
 * @author 董刚锋
 * page增加默认关注pojo
 *
 */
public class PagePojo implements Serializable{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private int pageId;
	private String pageName;
	private int universityId;
	private String universityName;
	private String recommendInfo;
	//0代表默认关注  1代表推荐关注
	private int pageType;
	private String remarks;
	
	
	public int getPageId() {
		return pageId;
	}
	public void setPageId(int pageId) {
		this.pageId = pageId;
	}
	public String getPageName() {
		return pageName;
	}
	public void setPageName(String pageName) {
		this.pageName = pageName;
	}
	public int getUniversityId() {
		return universityId;
	}
	public void setUniversityId(int universityId) {
		this.universityId = universityId;
	}
	public String getUniversityName() {
		return universityName;
	}
	public void setUniversityName(String universityName) {
		this.universityName = universityName;
	}
	public String getRecommendInfo() {
		return recommendInfo;
	}
	public void setRecommendInfo(String recommendInfo) {
		this.recommendInfo = recommendInfo;
	}
	public int getPageType() {
		return pageType;
	}
	public void setPageType(int pageType) {
		this.pageType = pageType;
	}
	public String getRemarks() {
		return remarks;
	}
	public void setRemarks(String remarks) {
		this.remarks = remarks;
	}
	
	

}
