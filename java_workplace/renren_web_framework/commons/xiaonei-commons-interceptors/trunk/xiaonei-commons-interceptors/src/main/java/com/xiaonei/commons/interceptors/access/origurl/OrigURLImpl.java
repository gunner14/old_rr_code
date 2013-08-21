package com.xiaonei.commons.interceptors.access.origurl;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

public class OrigURLImpl implements OrigURL {

    private static final String SKIP = "skip";

    private Invocation inv;

    private String value;

    public OrigURLImpl(Invocation inv) {
        this.inv = inv;
    }

    public boolean hasValue() {
        return value() != null;
    }

    @Override
    public String toString() {
        return value();
    }

    @Override
    public String value() {
        if (SKIP == value) {
            return null;
        }
        if (value == null) {
            HttpServletRequest request = inv.getRequest();
            String pureURL = request.getRequestURL().toString();
            if (pureURL != null) {
                String ourUrl = pureURL;
                if (request.getQueryString() != null) {
                    ourUrl = pureURL + "?" + urlSecurityEscape(request.getQueryString());
                }
                value = ourUrl;
            } else {
                value = SKIP;
                return null;
            }
        }
        return value;
    }

    /**
     * 将url中的危险字符转义
     * 
     * @param url
     * @return
     */
    protected String urlSecurityEscape(String url) {
        if (url == null) {
            return null;
        }
        StringBuilder sb = new StringBuilder(url.length());
        for (int i = 0; i < url.length(); i++) {
            char c = url.charAt(i);
            if (c == '<') {
                sb.append("&lt;");
            } else if (c == '>') {
                sb.append("&gt;");
            } else if (c == '"') {
                sb.append("&quot;");
            } else if (c == '\'') {
                //这个有风险，所以滤掉
            } else {
                sb.append(c);
            }
        }
        return sb.toString();
    }

}
