package com.dodoyo.controllers;

import java.io.IOException;
import java.util.Calendar;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

/**
 * <action path="/showCitySchool"
 * type="com.dodoyo.controllers.ClassFangleShowCitySchoolAction"
 * scope="request" /> 班级新鲜事，根据省、城市得到高中列表
 * 
 * @author Admin
 */

@Path("/showCitySchool.do")
public class ClassFangleShowCitySchoolController {

    @Get
    public String get(Invocation invocation) {
        // 院系新鲜事

        HttpServletRequest request = invocation.getRequest();
        String city = request.getParameter("city");// city 城市
        String type = request.getParameter("type");
        String ver = "" + Calendar.getInstance().get(Calendar.YEAR)
                + Calendar.getInstance().get(Calendar.MONTH)
                + Calendar.getInstance().get(Calendar.DATE);
        // 找到某城市高中列表存放的位置
        try {
            String path = null;
            if ("HIGHSCHOOL".equals(type)) {
                path = "/pages/school/highschool/";
            } else if ("OPENHIGHSCHOOL".equals(type)) {
                path = "/pages/school/openhighschool/";
            } else if ("COLLEGESCHOOL".equals(type)) {
                path = "/pages/school/collegeschool/";
            } else if ("JUNIORSCHOOL".equals(type)) {
                path = "/pages/school/juniorschool/";
            } else {
                path = "/pages/highschool/";
            }
            HttpServletResponse response = invocation.getResponse();
            response.sendRedirect(new StringBuilder().append("http://").append(
                    request.getServerName()).append(path).append(city).append(".html?ver=").append(
                    ver).toString());
        } catch (IOException e) {
            return null;
        }
        return null;
    }

}
