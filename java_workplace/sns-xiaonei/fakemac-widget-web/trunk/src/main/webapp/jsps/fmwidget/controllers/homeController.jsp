
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetDictModel"%>
<%@page import="com.xiaonei.wService.fmwidget.common.data.model.FakemacWidgetRecordModel"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.wService.fmwidget.adapter.FakemacWidgetAdapter"%>
<%@page import="com.xiaonei.platform.core.opt.base.PassportManager"%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<%
System.out.println(" homeController.jsp");
Integer chostid = PassportManager.getInstance().getHostIdFromCookie(request, response);
if(chostid == null || chostid == 0){
    out.println("login required");
    return;
}
request.setAttribute("hostid",chostid);
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


//List<FakemacWidgetRecordModel> mlist = FakemacWidgetAdapter.getInstance().selectFakemacWidgetRecordByUserid(userid);
javax.servlet.RequestDispatcher dis = request.getRequestDispatcher("/jsps/fmwidget/views/setting.jsp");
dis.forward(request,response);
%>
