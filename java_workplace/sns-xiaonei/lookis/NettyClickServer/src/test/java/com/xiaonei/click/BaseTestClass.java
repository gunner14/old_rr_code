/**
 * 
 */
package com.xiaonei.click;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.util.Map.Entry;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public abstract class BaseTestClass {

    protected Random r = new Random();

    protected Map<String, String> getRandomRightParam() {
        int count = Math.abs(r.nextInt(10)) + 1;
        Map<String, String> ret = new HashMap<String, String>();
        for (int i = 0; i < count; i++) {
            String key = "" + getRandomString();
            String value = "" + getRandomString();
            ret.put(key, value);
        }
        return ret;

    }

    protected String getRandomString() {
        int count = Math.abs(r.nextInt(10)) + 5;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < count; i++) {
            sb.append((char) ('a' + Math.abs(r.nextInt(26))));
        }
        return sb.toString();
    }

    protected String paramMapToString(Map<String, String> p, boolean encode)
            throws UnsupportedEncodingException {

        StringBuilder sb = new StringBuilder();
        for (Entry<String, String> entry : p.entrySet()) {
            sb.append(encode ? URLEncoder.encode(entry.getKey(), "UTF-8") : entry.getKey());
            sb.append("=");
            sb.append(encode ? URLEncoder.encode(entry.getValue(), "UTF-8") : entry.getValue());
            sb.append("&");
        }
        if (sb.toString().endsWith("&")) {
            sb.deleteCharAt(sb.length() - 1);
        }
        return sb.toString();
    }

}
