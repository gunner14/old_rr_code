package com.xiaonei.platform.taglib.sysmsg;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.core.sysmsg.SysMsg;
import com.xiaonei.platform.core.sysmsg.SysMsgManager;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 从cookie中得到系统输出的消息
 */

public class SysMsgTag extends TagSupport{	
	private static final long serialVersionUID = 1L;

	@Override
	public int doStartTag() throws JspException {
		HttpServletRequest request = (HttpServletRequest) pageContext.getRequest();
		HttpServletResponse response = (HttpServletResponse) pageContext.getResponse();
		SysMsg msg = SysMsgManager.getInstance().getSysMsg(request, response);
		if (msg == null) {
			return super.doStartTag();
		}
		
		int msgType = msg.getMsgType();
		String className;
		if (msgType == SysMsgManager.MSG_TYPE_ERROR) {
			className = "errors_div";
		}
		else if (msgType == SysMsgManager.MSG_TYPE_WARNING) {
			className = "messages_div";
		}
		else {
			return super.doStartTag();
		}
		String msgString = msg.getMsg();
		String msgDiv = "<div class=\"" + className + "\"><p>" + msgString + "</p></div>";
		TagUtils.out(pageContext, msgDiv);		
		
		return super.doStartTag();
	}	
}
