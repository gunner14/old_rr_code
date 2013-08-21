package com.xiaonei.platform.taglib.escapes;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.JspWriter;
import javax.servlet.jsp.tagext.TagSupport;

import org.apache.commons.lang.StringEscapeUtils;

public class EscapesJS extends TagSupport {
	private String value = null;

	public void setValue(String value) {
		this.value = value;
	}

	public int doEndTag() throws JspException {
		try {
			JspWriter out = pageContext.getOut();
			String unhtml = StringEscapeUtils.unescapeHtml(value);
			String js = StringEscapeUtils.escapeJavaScript(unhtml);
			// 在JS中的双引号是'\"',这个会影响HTML的解析,所以,在这里,对双引号特殊处理一下
			String result = js.replaceAll("\\\"", "&quot;");
			out.print(result);
		} catch (Exception e) {
			throw new JspException(e);
		}
		return EVAL_PAGE;
	}

	public static void main(String[] args) {
		// String content =
		// "\" It's me <html> \\ &#39; &nbsp; hello world <CR> <LF> <BS>\"";
		// System.out.println(content);
		// System.out.println();
		String content = "\\\"";
		System.out.println(content);
		System.out.println(content.replaceAll("\\\"", "&quot;"));
	}
}