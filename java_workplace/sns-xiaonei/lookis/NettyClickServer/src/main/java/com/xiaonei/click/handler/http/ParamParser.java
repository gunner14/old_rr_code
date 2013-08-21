/**
 * 
 */
package com.xiaonei.click.handler.http;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.HashMap;
import java.util.Map;

import org.jboss.netty.handler.codec.http.HttpRequest;

/**
 * 参数解析
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class ParamParser implements HttpParser {

    @Override
    public Map<String, String> parse(HttpRequest req) throws UnsupportedEncodingException {
        String uri = req.getUri();
        Map<String, String> param = new HashMap<String, String>();

        if (uri.indexOf('?') != -1) {//解析URI部分
            String uriP = uri.substring(uri.indexOf('?') + 1);
            if (uriP != null && uriP.trim().length() > 0) {
                param.putAll(paramString(uriP));
            }
        }
        //解析body部分
        if (req.getContentLength() > 0) {
            String bodyP = req.getContent().toString("UTF-8");
            if (bodyP != null && bodyP.trim().length() > 0) {
                param.putAll(paramString(bodyP));
            }
        }
        return param;

    }

    private Map<String, String> paramString(String reqString) throws UnsupportedEncodingException {
        Map<String, String> param = new HashMap<String, String>();
        for (String p : reqString.split("&")) {
            if (p.indexOf('=') != -1) {
                String k = URLDecoder.decode(p.substring(0, p.indexOf('=')), "UTF-8");
                String v = URLDecoder.decode(p.substring(p.indexOf('=') + 1), "UTF-8");
                param.put(k, v);
            }
        }
        return param;
    }
}
