package com.xiaonei.login.client;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONObject;

import com.xiaonei.platform.component.application.home.AppActiveUserLogHome;
import com.xiaonei.platform.component.application.model.AppActiveUserLog;
import com.xiaonei.platform.core.opt.base.PassportManager;

public class ApiLoginUtil {

    public static String getErrorInfo(final int code, final String msg, final boolean isJson) {
        if (isJson) {
            final JSONObject object = new JSONObject();
            try {
                object.put("error_code", code);
                object.put("error_msg", msg);
            } catch (final Exception e) {
                e.printStackTrace();
            }
            return object.toString();
        } else {
            final StringBuffer xml = new StringBuffer();
            xml.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            xml.append("<error_response>");
            xml.append("<error_code>");
            xml.append(code);
            xml.append("</error_code>");
            xml.append("<error_msg>");
            xml.append(msg);
            xml.append("</error_msg>");
            xml.append("</error_response>");
            return xml.toString();
        }
    }

    /**
     * 判断传入参数format的值是 json 或是 xml
     * 
     * @param request
     * @return
     */
    public static boolean isJsonFormat(final HttpServletRequest request) {
        final String format = request.getParameter("format");// 数据格式
        boolean isJson = false;
        if ("json".equalsIgnoreCase(format)) {
            isJson = true;
        }
        return isJson;
    }

    public static void sendResultResponse(final HttpServletResponse response,
            final String result, final boolean isJson) {
        // 将生成的字符串发往客户端
        if (isJson) {
            response.setContentType("text/plain; charset=UTF-8");
        } else {
            response.setContentType("text/xml; charset=UTF-8");
        }
        response.setHeader("Cache-Control", "no-cache");
        try {
            response.getWriter().write(result);
            response.getWriter().flush();
        } catch (final Exception e1) {
            // 异常处理代码略
            e1.printStackTrace();
        }
    }

    /**
     * 将L.do&CL.do的用户登录数据记录到日活跃中
     * 
     * @param userId
     * @param appId
     * @param appActiveType
     * @param request
     * @param response
     */
    @SuppressWarnings("deprecation")
    public static void updateActiveUserStat(final int userId, final int appId,
            final int appActiveType, final HttpServletRequest request,
            final HttpServletResponse response) {
        final AppActiveUserLog userLog = new AppActiveUserLog();
        userLog.setAppId(appId);
        userLog.setType(appActiveType);
        userLog.setUserId(userId);
        AppActiveUserLogHome.getInstance().stat(userId, appId, "", request,
                response, false, appActiveType);
    }

    /**
     * Api 验票方法
     * 
     * @param request
     * @param response
     * @return 验票成功返回hostid；否则返回null
     */
    public static Integer validatePassportTicket(final HttpServletRequest request,
            final HttpServletResponse response) {
        Integer hostid = null;
        hostid = PassportManager.getInstance().getHostIdFromCookie(request,
                response, PassportManager.loginKey_ticket);

        return hostid;
    }

}
