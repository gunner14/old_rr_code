
<%@page import="com.xiaonei.wService.fmwidget.common.util.MyStringUtils"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<%
System.out.println(" setController.jsp");
Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
if(chostid == null || chostid == 0){
    out.println("login required");
    return;
}

String id = request.getParameter("id");
int id_n = (Integer)MyStringUtils.stringToObject(id,Integer.class);
if(id_n == 0){
    out.println("app_id required");
    return;
}
String content = request.getParameter("content");
FakemacWidgetAdapter.getInstance().updateFakemacWidgetRecordSetContentById(chostid,id_n,content);

response.sendRedirect("/demo/home");
%>
