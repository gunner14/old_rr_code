package com.xiaonei.reg.register.model;

import java.io.Serializable;

import com.xiaonei.antispam.model.CheckResult;

public class AntispamCheckResult implements Serializable {

	private static final long serialVersionUID = -1913935269140516168L;
	private String M_content;
	private String M_info;
	private int M_type;

	private AntispamCheckResult(CheckResult cr) {

		this.M_type = cr.getFlag();
		this.M_info = cr.getHighLightContent();
		this.M_content = cr.getSafeRichContent();
	}
	
	public static AntispamCheckResult cover(CheckResult cr){
		if(cr == null){
			return null;
		}
		return new AntispamCheckResult(cr);
	}

	public String getM_content() {
		return M_content;
	}

	public String getM_info() {
		return M_info;
	}

	public int getM_type() {
		return M_type;
	}

}
