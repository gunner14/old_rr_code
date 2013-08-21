package com.renren.sns.wiki.utils;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import net.sf.json.JSONObject;
import net.sf.json.util.JSONUtils;

/**
 * 
 * @author yi.li@renren-inc.com since 2012-4-12
 * 
 */
public class WebJSONUtil {

    private final static String SINGLE_QUOTE = "'";

    private static String singleQuote(String string) {
        if (string == null || string.length() == 0) {
            return SINGLE_QUOTE + SINGLE_QUOTE;
        }

        char b;
        char c = 0;
        int i;
        int len = string.length();
        StringBuilder sb = new StringBuilder(len * 2);
        String t;
        char[] chars = string.toCharArray();
        char[] buffer = new char[1030];
        int bufferIndex = 0;
        sb.append(SINGLE_QUOTE);
        for (i = 0; i < len; i += 1) {
            if (bufferIndex > 1024) {
                sb.append(buffer, 0, bufferIndex);
                bufferIndex = 0;
            }
            b = c;
            c = chars[i];
            switch (c) {
                case '\\':
                case '"':
                case '\'':
                    buffer[bufferIndex++] = '\\';
                    buffer[bufferIndex++] = c;
                    break;
                case '/':
                    if (b == '<') {
                        buffer[bufferIndex++] = '\\';
                    }
                    buffer[bufferIndex++] = c;
                    break;
                default:
                    if (c < ' ') {
                        switch (c) {
                            case '\b':
                                buffer[bufferIndex++] = '\\';
                                buffer[bufferIndex++] = 'b';
                                break;
                            case '\t':
                                buffer[bufferIndex++] = '\\';
                                buffer[bufferIndex++] = 't';
                                break;
                            case '\n':
                                buffer[bufferIndex++] = '\\';
                                buffer[bufferIndex++] = 'n';
                                break;
                            case '\f':
                                buffer[bufferIndex++] = '\\';
                                buffer[bufferIndex++] = 'f';
                                break;
                            case '\r':
                                buffer[bufferIndex++] = '\\';
                                buffer[bufferIndex++] = 'r';
                                break;
                            default:
                                t = "000" + Integer.toHexString(c);
                                int tLength = t.length();
                                buffer[bufferIndex++] = '\\';
                                buffer[bufferIndex++] = 'u';
                                buffer[bufferIndex++] = t.charAt(tLength - 4);
                                buffer[bufferIndex++] = t.charAt(tLength - 3);
                                buffer[bufferIndex++] = t.charAt(tLength - 2);
                                buffer[bufferIndex++] = t.charAt(tLength - 1);
                        }
                    } else {
                        buffer[bufferIndex++] = c;
                    }
            }
        }
        sb.append(buffer, 0, bufferIndex);
        sb.append(SINGLE_QUOTE);
        return sb.toString();
    }

    /**
     * 将JSONObject转化为使用单引号的JSON字符串{'a':'b'}
     * 
     * @param json
     * @return 转化后的JSON字符串
     */
    @SuppressWarnings("rawtypes")
    public static String formatSingleQuoteJSON(JSONObject json) {
        if (null == json) {
            return "{}";
        }

        Iterator keys = json.keys();
        Object o = null;
        String s;
        StringBuffer sb = new StringBuffer();

        sb.append('{');
        while (keys.hasNext()) {
            if (o != null) {
                sb.append(',');
            }
            s = keys.next().toString();
            o = json.get(s);
            if (o != null) {
                sb.append(singleQuote(s));
                sb.append(':');
                sb.append(valueToString(o));
            }
        }
        sb.append('}');

        return sb.toString();
    }

    @SuppressWarnings("rawtypes")
    private static String valueToString(Object value) {
        if (value instanceof Number) {
            return JSONUtils.numberToString((Number) value);
        } else if (value instanceof Boolean) {
            return value.toString();
        } else if (value instanceof Map) {
            return formatSingleQuoteJSON((JSONObject) value);
        } else if (value instanceof List) {
            List arr = (List) value;
            StringBuilder sb = new StringBuilder();
            sb.append('[');
            int len = arr.size();
            for (int i = 0; i < len; i += 1) {
                if (i > 0) {
                    sb.append(",");
                }
                String arrvalue = valueToString(arr.get(i));
                sb.append(arrvalue);
            }
            sb.append(']');

            return sb.toString();
        } else {
            return singleQuote(value.toString());
        }

    }

    public static void main(String[] args) {
        JSONObject json = new JSONObject();
        List<String> ss = new ArrayList<String>();
        ss.add("adfd");
        ss.add("adxxx");
        json.put("hello", "&lt;/script&gt;'''''\"&quot;&#39;&amp;");
        json.put("ss1", true);
        json.put("ss", 123);
        json.put("ss3", ss);
        System.out.println(formatSingleQuoteJSON(json));
        System.out.println(json);
    }
}
