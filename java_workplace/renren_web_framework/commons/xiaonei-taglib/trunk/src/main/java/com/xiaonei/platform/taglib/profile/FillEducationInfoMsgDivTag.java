package com.xiaonei.platform.taglib.profile;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.component.profile.logic.ProfileLogic;
import com.xiaonei.platform.taglib.utils.TagUtils;

public class FillEducationInfoMsgDivTag extends TagSupport {

	private int userId;

	private boolean isShow = false;

	@Override
	public int doStartTag() throws JspException {
		try {
			isShow = !ProfileLogic.isEducationInfoFull(userId);
		} catch (Exception e) {
			isShow = false;
		}
		if (isShow) {
			TagUtils
					.out(
							pageContext,
							"<div id=\"info\"><div id=\"dashNoticeyellow\"><p style=\"margin:10px; text-align:left;\">");
			return EVAL_BODY_INCLUDE;
		} else {
			return SKIP_BODY;
		}
	}

	@Override
	public int doEndTag() throws JspException {
		if (isShow) {
			TagUtils.out(pageContext, "</p></div></div>");
		}
		return SKIP_BODY;
	}

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}
}
