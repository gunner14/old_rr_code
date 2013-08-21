package com.xiaonei.platform.taglib.url;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import org.apache.taglibs.standard.lang.support.ExpressionEvaluatorManager;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * Main_Url 主页上大头像，user_desc表中都有mainurl字段
 */

public class UserMainUrlTag extends TagSupport{
	
	private Object url;

	public void setUrl(Object url) throws JspException {
		this.url = ExpressionEvaluatorManager.evaluate("url",url.toString(),Object.class,this.pageContext);
	}


	@Override
	public int doStartTag() throws JspException {
		if(url!=null){	
			TagUtils.out(pageContext,HeadUrlUtil.getHeadFullUrl(url.toString()));
		}
		return super.doStartTag();
	}

}
