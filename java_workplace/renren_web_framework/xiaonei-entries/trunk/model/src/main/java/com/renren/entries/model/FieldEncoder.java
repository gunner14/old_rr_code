package com.renren.entries.model;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;


/**
 * 封装对Field的name和value就行编码的逻辑
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-7-5 下午04:06:02
 */
public class FieldEncoder {

    private static final String ENCODED_EQUAL = urlEncode("=");

    private static final String ENCODED_AND = urlEncode("&");

    private static final String ENCODED_DOLLAR = urlEncode("$");

    private static final String ENCODED_PERCENT = urlEncode("%");
    
    /**
     * 与encode方法对应的decode方法，使用URL-Decode就行
     * 
     * @param value
     * @return
     */
    static String decodeField(String value) {
        
        StringBuilder sb = new StringBuilder(value.length());
        boolean changed = false;
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            if (c == '%' && i + 2 < value.length()) {   //i后面至少还有两个字符
                String seq = value.substring(i, i + 3); //取%及其后面的两个字符
                if (seq.equals(ENCODED_EQUAL)) {
                    sb.append("=");
                    i += 2;
                    changed = true;
                } else if (seq.equals(ENCODED_AND)) {
                    sb.append("&");
                    i += 2;
                    changed = true;
                } else if (seq.equals(ENCODED_DOLLAR)) {
                    sb.append("$");
                    i += 2;
                    changed = true;
                } else if (seq.equals(ENCODED_PERCENT)) {
                    sb.append("%");
                    i += 2;
                    changed = true;
                } else {
                    sb.append(c);
                }
            } else {
                sb.append(c);
            }
        }
        if (changed) {
            return sb.toString();
        } else {
            return value;
        }
    }

    /**
     * 只对"=&$%"这四个保留字符进行URL编码，其他字符保持不变
     * 
     * @param value
     * @return
     */
    static String encodeField(String value) {
        StringBuilder ret = new StringBuilder(value.length() + 20);
        boolean encoded = false;
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            switch (c) {
                case '=':   //=是name和value的分隔符，需要编码
                    ret.append(ENCODED_EQUAL);
                    encoded = true;
                    break;
                case '&':   //&是field之间的分隔符，需要编码
                    ret.append(ENCODED_AND);
                    encoded = true;
                    break;
                case '$':   //$是索引的标识符，需要编码
                    ret.append(ENCODED_DOLLAR);
                    encoded = true;
                    break;
                case '%':   //上述三个字符编码后的字符串里会出现%，所以%也要被编码
                    ret.append(ENCODED_PERCENT);
                    encoded = true;
                    break;
                default:
                    ret.append(c);
                    break;
            }
        }
        return encoded ? ret.toString() : value;
    }
    
    /**
     * URL编码
     * 
     * @param value
     * @return
     */
    private static final String urlEncode(String value) {
        try {
            return URLEncoder.encode(value, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            throw new Error(e);
        }
    }

}
