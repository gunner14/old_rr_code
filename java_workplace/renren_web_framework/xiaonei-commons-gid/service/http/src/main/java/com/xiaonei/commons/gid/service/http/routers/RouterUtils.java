package com.xiaonei.commons.gid.service.http.routers;

import javax.servlet.http.HttpServletRequest;

public class RouterUtils {

    public static long extractUserParameter(HttpServletRequest request) {
        String user = request.getParameter("u");
        if (user == null) {
            throw new NullPointerException("there's no 'u' parameter in the request");
        }
        return Long.parseLong(user);
    }
}
