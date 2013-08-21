package com.xiaonei.platform.core.utility;


/**
 * 
 * Escape.java
 * @author drizzt(drizzt.wang@gmail.com) 2006-2-28 5:07:24
 */
public class Escape {
//	public static String banned = ".*(5q|baodoo|dipian|5jia1|yeejee|faceben|xiaonei|校内网).*";
	public static String banned;
	public static String filter = ".*faceben.*";
	
	static{
		if (com.xiaonei.platform.core.opt.OpiConstants.isXiaonei()) {
			banned = ".*(5q|baodoo|dipian|5jia1|yeejee|faceben|chinay|faceren|校内网).*";
		} else {
			banned = ".*(5q|baodoo|dipian|5jia1|yeejee|faceben|chinay|faceren|开心网).*";
		}
	}
	/**
	 * 上载后再编辑就会有问题，显示不出来
	 * @param string
	 * @return
	 */
	public static String stringToHTMLString(String string) {
		if (string == null) {
			return null;
		}
		StringBuffer sb = new StringBuffer(string.length());
		// true if last char was blank
//		boolean lastWasBlankChar = false;
		int len = string.length();
		char c;

		for (int i = 0; i < len; i++) {
			c = string.charAt(i);
//			// blank gets extra work,
//			// this solves the problem you get if you replace all
//			// blanks with &nbsp;, if you do that you loss 
//			// word breaking
//			if (lastWasBlankChar) {
//				lastWasBlankChar = false;
//				sb.append("&nbsp;");
//			} else {
//				lastWasBlankChar = true;
//				sb.append(' ');
//			}
			switch (c) {
			case '<': sb.append("&lt;"); break;
			case '>': sb.append("&gt;"); break;
			case '&': sb.append("&amp;"); break;
			case '#': sb.append("&#35;"); break;
//			case '"': sb.append("&quot;"); break;
			// be carefull with this one (non-breaking whitee space)
//			case ' ': sb.append("&nbsp;");break;
			case '(': sb.append("&#40;"); break;
			case ')': sb.append("&#41;"); break;
			case '"': sb.append("&#34;"); break;
			case '\'': sb.append("&#39;"); break; 
			default:  sb.append(c); break;
			}
		}
		

		return sb.toString();
    }
	
	 public static String decodeHTML(String InputString)
	    {
	        String OutputString = null;
	        if(InputString == null)
	        {
	            return null;
	        } else
	        {
	            OutputString = new String(InputString);
	            OutputString = OutputString.replace("&amp;", "&");
	            OutputString = OutputString.replace("&lt;", "<");
	            OutputString = OutputString.replace("&gt;", ">");
	            OutputString = OutputString.replace("&quot;", "\"");
	            OutputString = OutputString.replace("&nbsp;", " ");
	            return OutputString;
	        }
	    }
	
	public static boolean isValidLink(String string) {
		if (string == null) {
			return false;
		}
		
		char c;
		for (int i = 0; i < string.length(); i++) {
			c = string.charAt(i);
			if (!Character.isDigit(c)
					&& !Character.isLetter(c)) {
				return false;
			}
		}
		return true;
	}
	
	public static boolean isLetterExist(String string) {
		if (string == null) {
			return false;
		}
		
		char c;
		for (int i = 0; i < string.length(); i++) {
			c = string.charAt(i);
			if (Character.isLetter(c)) {
				return true;
			}
		}
		return false;
	}
	
	
	
	static final String[] script_editor = {"onabort", "onblur", "onchange", "onclick", "ondblclick", 
		"onerror", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onload", "onmousedown", "onmousemove",
		"onmouseout", "onmouseover", "onmouseup", "onreset", "onresize", "onselect", "onsubmit", "onunload", 
		"<script", "<frame", "<applet", "<object", "<form", "<meta", "location"};
	
	public static String checkEditorHtml(final String str) {
		if (str == null) {
			return null;
		}
		
		String st = str.toLowerCase();
		for (int i = 0; i < script_editor.length; i++) {
			if (st.indexOf(script_editor[i]) > -1) {
				return script_editor[i]; 
			}
		}
		return null;
	}
	
	static final String[] script_space = {"onabort", "onblur", "onchange", "onclick", "ondblclick", "onerror", "onfocus", "onkeydown", "onkeypress", "onkeyup", "onload", "onmousedown", "onmousemove", "onmouseout", "onmouseover", "onmouseup", "onreset", "onresize", "onselect", "onsubmit", "onunload", "script", "frame", "applet",  "form", /*"&",*/ "meta", "location"};
	
	public static String checkSpaceHtml(final String str) {
		if (str == null) {
			return null;
		}
		
		String st = str.toLowerCase();
		for (int i = 0; i < script_space.length; i++) {
			if (st.indexOf(script_space[i]) > -1) {
				return script_space[i]; 
			}
		}
		return null;
	}
	
	public static String trim(final String str) {
		if (str == null) {
			return null;
		}
		
		String st = str.trim();
		while (st.length() > 0 && st.charAt(0) == '　') {
			st = st.substring(1);
		}
		while (st.length() > 0 && st.charAt(st.length() - 1) == '　') {
			st = st.substring(0, st.length() - 1);
		}
		return st;
	}
	
	public static String purgeWhiteSpace(String st) {
		if (st == null) {
			return null;
		}
		StringBuffer sb = new StringBuffer(st.length());
		int len = st.length();
		char c;

		for (int i = 0; i < len; i++) {
			c = st.charAt(i);
			if (!Character.isWhitespace(c)) {
				sb.append(c);
			}
		}
		return sb.toString();
	}
}