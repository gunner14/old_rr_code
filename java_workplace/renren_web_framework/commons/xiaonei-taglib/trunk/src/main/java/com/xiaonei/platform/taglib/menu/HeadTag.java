package com.xiaonei.platform.taglib.menu;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.taglib.helper.TempletData;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class HeadTag extends TagSupport {


	@Override
	public int doStartTag() throws JspException {
		String t1 = TempletData.getInstance().getMap(TempletData.Head_Meta);
		String t2 = TempletData.getInstance().getMap(TempletData.Head_Css);
		String t3 = TempletData.getInstance().getMap(TempletData.Head_Javascript);
		t1 = t1==null?"":t1;
		t2 = t2==null?"":t2;
		t3 = t3==null?"":t3;
		TagUtils.out(pageContext, t1+t2+t3);
		return super.doStartTag();
	}
}