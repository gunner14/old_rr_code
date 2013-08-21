
<%@page import="com.xiaonei.wService.fmwidget.common.util.MyStringUtils"%>
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<%
System.out.println(" appinfoController.jsp");
Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
if(chostid == null || chostid == 0){
    out.println("login required");
    return;
}
request.setAttribute("hostid",chostid);
String app_id = request.getParameter("app_id");
int app_id_n = (Integer)MyStringUtils.stringToObject(app_id,Integer.class);
List<FakemacWidgetDictModel> dlist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetDict(0,0,10);
for(FakemacWidgetDictModel curdict : dlist){
    if(curdict.getApp_id() == app_id_n){
        request.setAttribute("curdict",curdict);
        break;
    }
}


//List<FakemacWidgetRecordModel> mlist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetRecordByUserid(userid);
javax.servlet.RequestDispatcher dis = request.getRequestDispatcher("/jsps/fmwidget/views/appinfo.jsp");
dis.forward(request,response);
%>
