<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_doinginfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>


<%@page import="com.xiaonei.platform.core.opt.permission.define.RelationDef"%>
<%@page import="com.xiaonei.platform.component.doing.home.DoingHome"%>
<%@page import="com.xiaonei.platform.component.doing.model.Doing"%>
<%@page import="com.xiaonei.platform.component.doing.model.ReplyDoing"%>
<%@page import="com.xiaonei.platform.component.doing.model.DoingUBB"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Doing Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    String doing = request.getParameter("doing");
    if(MyStringUtils.equals("on",doing)){
	    int cnt = DoingHome.getInstance().getDoingCount(host.getId());
	    String curout = String.format("<li>Doing Count:%s</li>",cnt);
	    out.println(curout);
    	Doing lastdoing = DoingHome.getInstance().getUserLastDoing(host.getId());
    	if(lastdoing!=null){
	    	curout = String.format("<li>[%s]%s(%s)%s</li>",lastdoing.getDoTime().toString(),lastdoing.getHtmlContentForGuide(),lastdoing.getCommentCount()+"",lastdoing.getIp());
	    	out.println(curout);
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