package com.xiaonei.platform.taglib.escapes;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.JspWriter;
import javax.servlet.jsp.tagext.TagSupport;

import org.apache.commons.lang.StringEscapeUtils;

public class EscapesHTML extends TagSupport {
	private String value = null;

	public void setValue(String value) {
		this.value = value;
	}

	public int doEndTag() throws JspException {
		try {
			JspWriter out = pageContext.getOut();
			String unhtml = StringEscapeUtils.unescapeHtml(value);
			String html = StringEscapeUtils.escapeHtml(unhtml);
			out.print(html);
		} catch (Exception e) {
			throw new JspException(e);
		}
		return EVAL_PAGE;
	}

	public static void main(String[] args) {
		String content = "\" It's me <html> \\ &nbsp; hello world 林夕'xi♥゛\"";
		System.out.println("content ==> " + content);
		System.out.println("html ==> "
				+ StringEscapeUtils.escapeHtml(content));
		System.out.println("js ==> "
				+ StringEscapeUtils.escapeJavaScript(content));
		System.out.println("unjs ==> "
				+ StringEscapeUtils.unescapeJavaScript(StringEscapeUtils
						.escapeJavaScript(content)));
	}
}