package com.xiaonei.xnmonitor.dto;

import java.util.LinkedList;
import java.util.List;

public class GroupedDomains {
	private String domain;
	private List<String> errTypes=new LinkedList<String>();

	public String getDomain() {
		return domain;
	}

	public void setDomain(String domain) {
		this.domain = domain;
	}

	public List<String> getErrTypes() {
		return errTypes;
	}

	public void setErrTypes(List<String> errTypes) {
		this.errTypes = errTypes;
	}

}
