package com.xiaonei.platform.component.tools;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PureTextUtil {
	
	public static String filterScript(String content) {
		String reg = "<[\\s]*?script[^>]*?>[\\s\\S]*?<[\\s]*?\\/[\\s]*?script[\\s]*?>";
		Pattern p_script = Pattern.compile(reg, Pattern.CASE_INSENSITIVE);
		Matcher m_script = p_script.matcher(content);
		content = m_script.replaceAll(""); // 过滤script标签
		return content;
	}
}
