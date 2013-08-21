/**
 * 
 */
package com.xiaonei.reg.register.pojo;

import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;

/**
 * @author wangtai
 * 
 */
public class RegFromSourcePojo {

	private int userId;
	private String actionId;
	private Class<? extends BaseRegSubmitController> formType;
	private String pageUuid;
	private String ip;
	private int accType;

	public static RegFromSourcePojo getNew(
			Class<? extends BaseRegSubmitController> formType,int userId) {
		return new RegFromSourcePojo(formType,userId);
	}

	private RegFromSourcePojo(Class<? extends BaseRegSubmitController> formType, int userId) {
		this.formType = formType;
		this.userId = userId;
	}

	public String getActionId() {
		return actionId;
	}

	public RegFromSourcePojo setActionId(String actionId) {
		this.actionId = actionId;
		return this;
	}

	public Class<? extends BaseRegSubmitController> getFormType() {
		return formType;
	}

	public String getPageUuid() {
		return pageUuid;
	}

	public RegFromSourcePojo setPageUuid(String pageUuid) {
		this.pageUuid = pageUuid;
		return this;
	}

	public RegFromSourcePojo setIp(String ip) {
		this.ip = ip;
		return this;
	}

	public String getIp() {
		return ip;
	}

	public RegFromSourcePojo setAccType(int accType) {
		this.accType = accType;
		return this;
	}

	public int getAccType() {
		return accType;
	}

	public int getUserId() {
		return userId;
	}

}
