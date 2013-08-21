
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.wService.fmwidget.common.util.MyStringUtils"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<%
System.out.println(" getController.jsp");
Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
if(chostid == null || chostid == 0){
    out.println("login required");
    return;
}

String id = request.getParameter("id");
int id_n = (Integer)MyStringUtils.stringToObject(id,Integer.class);
if(id_n == 0){
    List<FakemacWidgetRecordModel> mlist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetRecordByUserid(chostid);
    List<FakemacWidgetRecordModel> tobedel = new ArrayList<FakemacWidgetRecordModel>();
    for(FakemacWidgetRecordModel curm : mlist){
        if(curm.isDelMark()){
            tobedel.add(curm);
        }
    }
    mlist.removeAll(tobedel);
    List<FakemacWidgetDictModel> dlist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetDict(0,0,10);
    System.out.println(" records:"+mlist.size()+" dict:"+dlist.size());
    request.setAttribute("records",mlist);
    request.setAttribute("dicts",dlist);
}
else{
	String content = request.getParameter("content");
	List<FakemacWidgetRecordModel> mlist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetRecordByUserid(chostid);
	for(FakemacWidgetRecordModel curm : mlist){
	    if(curm.getId() == id_n){
	        request.setAttribute("curm",curm);
	        break;
	    }
	}
}

javax.servlet.RequestDispatcher dis = request.getRequestDispatcher("/jsps/fmwidget/views/getuserappcontent.jsp");
dis.forward(request,response);
%>
