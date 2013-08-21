package com.xiaonei.platform.core.opt.adminPermission.taglib;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.core.opt.adminPermission.AdminManager;

public class SecurityTag extends TagSupport {
	
	private int userId;
	
	private int functionId;
	
	private int right;
	
	public int doStartTag() throws JspException{
		if(AdminManager.getInstance().hasRight(userId,functionId, right)){
			return this.EVAL_BODY_INCLUDE;
		}
		else{
			return this.SKIP_BODY;
		}
	}

	public int getFunctionId() {
		return functionId;
	}

	public void setFunctionId(int functionId) {
		this.functionId = functionId;
	}

	public int getRight() {
		return right;
	}

	public void setRight(int right) {
		this.right = right;
	}

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

}
