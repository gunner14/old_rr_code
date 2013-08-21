<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_iploginfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.msg.home.MessageHome"%>
<%@page import="com.xiaonei.msg.view.MessageIn"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.xce.footprintunite.FootprintView"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.xce.scorecache.DefaultScoreCache"%>
<%@page import="com.xiaonei.xce.scorecache.ScoreCacheAdapter"%>
<%@page import="java.util.Calendar"%>
<%@page import="java.text.SimpleDateFormat"%>
<%@page import="java.text.DateFormat"%>
<%@page import="net.paoding.rose.scanning.context.RoseAppContext"%>
<%@page import="com.xiaonei.admin.biz.dao.component.IpLogDAO"%>
<%@page import="com.xiaonei.admin.biz.model.component.IpLog"%>
<%@page import="java.util.Date"%><div></div>
<%
//User host;
String iplog = request.getParameter("iplog");
String iplog_date = request.getParameter("iplog_date");
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null && MyStringUtils.equals("on",iplog) && isMan(chost.getId())){
%>

<!-- {------------ --> 
<h2 class="title">IP Log Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    if(!MyStringUtils.isNullOrEmpty(iplog_date)){
    %>
	<%
		int userId = host.getId();//用户id
	    
	    final DateFormat df = new SimpleDateFormat("yyyyMMdd");

	    RoseAppContext context = new RoseAppContext();
	    /*
	    IpAddDAO ipdao = context.getBean(IpAddDAO.class);
	    List<IpAdd> iplist = ipdao.getByIp("221.200.219.207",0,2);
	    for(IpAdd add : iplist){
	        out.println(add.getEmail()+"<br />");
	    }*/
	    IpLogDAO iplogdao = context.getBean(IpLogDAO.class);
	    List<IpLog> records = null;
	    StringBuilder tableName = new StringBuilder("iplog");
	    String date = iplog_date;
	    if (!date.equals(df.format(new Date()))) {
	        tableName.append("_").append(date);
	    }
	    try {
	        records = iplogdao.queryIpById(userId,tableName.toString(),null,null);
	    } catch (Exception e) {
	        e.printStackTrace();
	    }
	    for(IpLog log : records){
	        out.println(log.getIp()+"["+log.getTime()+"]<br />");
	    }
	%>
	 	    
    <%
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
<!-- ------------} --> 
<%
}
%>