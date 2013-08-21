package com.xiaonei.platform.core.utility;

/**
 * Process mail related things
 * 
 * Created on 2004-2-18
 *
 * @author Freebat Wangh<wanghuiwen@tsinghua.org.cn>
 */
public class RegularExpression {
    public static String processForSearch(String st) {
        StringBuffer buf = new StringBuffer();
        for (int i = 0; i < st.length(); i++) {
            char c = st.charAt(i);
            if (c == '[' || c == ']' || c == '{' || c == '}' || c == '(' || c == ')') {
            	if (i != 0 && i != st.length() - 1) {
            		buf.append('|');
            	}
            } else if (c == '\\' || c == '*' || c == '+' || c == '?' || c == ',' || c == '^' || c == '$' || c ==  '!' || c == '~' || c == '#' || c == '<' || c == '>' || c == ':' || c == ';' || c == '=') {                
            } else if (c == '.') {
            	if (i != 0 && i != st.length() -1 && st.charAt(i-1) != '.') {
            		buf.append('.');
            	}
            } else if (c == '|') {
            	if (i != 0 && i != st.length() - 1) {
            		buf.append('|');
            	}
            } else {
                buf.append(c);
            }
        }
        return buf.toString();
    }

    public static String processForLogin(String st) {
        StringBuffer buf = new StringBuffer();
        for (int i = 0; i < st.length(); i++) {
            char c = st.charAt(i);
            if (c == '\'' || c == '\"') {
            	continue;
            }
            buf.append(c);
        }
        return buf.toString();
    }

}