<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_pageinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>

<%@page import="com.xiaonei.platform.core.opt.permission.define.RelationDef"%>
<%@page import="com.xiaonei.page.service.PageService"%>
<%@page import="com.xiaonei.page.domain.Page"%>
<%@page import="com.xiaonei.page.domain.view.HomeSpreadView"%>
<%@page import="com.xiaonei.page.service.PageSpreadService"%>
<%@page import="com.xiaonei.page.service.AdminService"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Page Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    String pageon = request.getParameter("pageon");
    int n_pageid = host.getId();
    if(MyStringUtils.equals("on",pageon)){
    	Page puser = null;
    	try{
    		 puser = PageService.getInstance().get(n_pageid);
    	}
    	catch(Exception e){
    		out.println("<li>"+"n_pageid:"+n_pageid+" is not a page."+"</li>");
    	}
    	if(puser!=null){
	    	int ownerid = puser.getOwnerId();
	    	
	    	out.println("<li>"+"Owner Id:"+ownerid+"</li>");
	    	out.println("<li>"+"Owner Id:"+ownerid+"</li>");
	    	out.println("<li>"+"Fans Count:"+puser.getFansCount()+"</li>");
	    	out.println("<li>"+"Fans Counta:"+puser.getCounta()+"</li>");
	    	out.println("<li>"+"Fans Count Real:"+puser.getRealFansCount()+"</li>");
	    	out.println("<li>"+"Alias:"+puser.getAlias()+"</li>");
	    	out.println("<li>"+"Create Time:"+puser.getCreateTime()+"</li>");
	    	out.println("<li>"+"Description:"+puser.getDescription()+"</li>");
    	}
    	else{
    		
    	}
    	
    	HomeSpreadView homeSpreadView = PageSpreadService.getInstance().getHomeSpreads(host.getId());
    	List<Page> myPages = homeSpreadView.getMypages();
    	if(myPages != null && myPages.size()>0){
    		out.println("<li>");
	    	for(Page cur_p : myPages){
	    		out.println(cur_p.getId()+"["+ cur_p.getName()+"]"+"<br />");
	    	}
	    	out.println("</li>");
    	}
    	List<Page> myManPages = AdminService.getInstance().getManagedPages(host.getId(), 10);
    	if(myManPages != null && myManPages.size()>0){
            out.println("<li>");
            for(Page cur_p : myManPages){
                out.println(cur_p.getId()+"["+ cur_p.getName()+"]"+"<br />");
            }
            out.println("</li>");
        }
    	
    	
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
</div>
<!-- {------------ --> 
<%
}
%>