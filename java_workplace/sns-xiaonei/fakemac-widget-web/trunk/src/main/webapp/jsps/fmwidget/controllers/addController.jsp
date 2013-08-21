
<%@page import="com.xiaonei.wService.fmwidget.common.util.MyStringUtils"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<%
System.out.println(" addController.jsp");
Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
if(chostid == null || chostid == 0){
    out.println("login required");
    return;
}

String app_id = request.getParameter("app_id");
int app_id_n = (Integer)MyStringUtils.stringToObject(app_id,Integer.class);
if(app_id_n == 0){
    out.println("app_id required");
    return;
}
List<FakemacWidgetRecordModel> modellist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetRecordByUserid(chostid);
if(modellist.size() >= 20){
	request.setAttribute("id",0);
}
else{
	FakemacWidgetRecordModel m = new FakemacWidgetRecordModel();
	m.setAppId(app_id_n);
	int id = FakemacWidgetAdapter.getInstance().insertFakemacWidgetRecordAndGetId(chostid,m);
	System.out.println("id:"+id);
	request.setAttribute("id",id);
}

javax.servlet.RequestDispatcher dis = request.getRequestDispatcher("/jsps/fmwidget/views/add.jsp");
dis.forward(request,response);
%>
