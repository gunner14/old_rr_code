<%@page import="com.renren.sns.friend.model.HiddenFootprint"%>
<%@page import="com.renren.sns.friend.logic.HiddenFootprintLogic"%>
<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.model.UserTime"%>
<%@ page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserIdentityDAO"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.text.SimpleDateFormat"%>
<!-- sub_userinfo -->
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>
<!-- {------------ --> 
            

<%@page import="java.util.List"%>
<%@page import="java.util.ArrayList"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%><h2 class="title">User Basic Info</h2>
            <p class="byline"><small>Posted by inshion</small></p>
            <div class="entry">
	<%
	//User host;
	if(host!=null){
		//out.println(host.getId());
		String tinyurl = host.getTinyUrl();
		String fullurl = host.getHeadFullUrl();
		UserTime time = SnsAdapterFactory.getUserTimeAdapter().getUserTime(host.getId());
		int identity = GuideXFiveUseIdentityLogic.getInstance().getIdentity(host.getId());
	    int identityDao = GuideUserIdentityDAO.getInstance().getIdentity(host.getId());
	    SimpleDateFormat fmt = new SimpleDateFormat("yyyy-MM-dd=HH:mm:ss");
	    int iconsize = 16;
	    List<Integer> ids = new ArrayList<Integer>();
	    ids.add(host.getId());
	    Map<Integer, WUserCache> ucas = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(ids);
	    WUserCache uca = ucas.get(host.getId());
	    
	%>
            
	                <p><strong><%=host.getId() %></strong>(<%=host.getAccount() %>)[<%=host.getName() %><%=uca==null?".":uca.isOnline()?"<img width=\"13\" height=\"12\" src=\"http://xnimg.cn/imgpro/icons/online_1.gif\" title=\"在线用户\" class=\"on-line\">":"" %>]
	                 is a <%=host.getDomain() %> user. Orange:(<%=host.getUserState().isKeepUse() %>)<br />
	                 Below is his/her acount info.<!-- <font color="white"> <%=host.getPassword_md5()%></font> -->
	                </p>
	            <table width="80%">
		            <tr>
		                <td><img alt="用户性别" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/icons/statusface/13.gif" /></td><td><%=host.getGender() %></td>
		                <td rowspan="5" style="text-align:center;border:1px dotted green"><img src="<%=fullurl %>"</td>
		            </tr>
		            <tr>
		                <td><img alt="用户账号" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/emotions/tie/7.gif" /></td><td><%=host.getEmail() %></td>
		            </tr>
		            <tr>
		                <td><img alt="用户名字" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/icons/statusface/kbz2.gif" /></td><td><%=host.getName() %> <%=MyStringUtils.isNullOrEmpty(host.getNickname())?"":"["+host.getNickname()+"]" %></td>
		            </tr>
		            <tr>
		                <td><img alt="注册时间" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/icons/statusface/8.gif" /></td><td><%=fmt.format(time.getRegisterTime())%></td>
		            </tr>
		            <tr>
		                <td><img alt="激活时间" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/icons/statusface/11.gif" /></td><td><%=fmt.format(time.getActivateTime()) %></td>
		            </tr>
		            <tr>
		                <td><img alt="用户生日" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/emotions/tie/3.gif" /></td><td><%=ub.getBirthYear()+"-"+ub.getBirthMonth()+"-"+ub.getBirthDay()+""+"" %></td>
		                <td rowspan="3" style="text-align:center;border:1px dotted green"><img src="<%="http://hdn.xnimg.cn/photos/"+tinyurl %>"</td>
		            </tr>
		            <tr>
		                <td><img alt="用户阶段" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/emotions/tie/21.gif" /></td><td><%=host.getStage() %></td>
		            </tr>
		            <tr>
		                <td><img alt="阶段标识" width="<%=iconsize %>" height="<%=iconsize %>" src="http://a.xnimg.cn/imgpro/icons/statusface/15.gif" /></td><td><%=identity %>[<%=identityDao %>]</td>
		            </tr>
	            </table>
	        </div>
	        <%
	        out.println("<!-- <font color=white>"+fullurl+"<br />"+tinyurl+"</font> -->"); 
	        HiddenFootprint hfp = HiddenFootprintLogic.getInstance().getHiddenFootprint(host.getId());
	        List<Integer> hfps = hfp.list();
	        int index_hfps = 0;
	        for(int h:hfps){
	        	out.println("<li>"+index_hfps+++":"+h+"</li>");
	        }
	        %>
	        <!-- 
	        <div class="meta">
	                <p class="links"><a href="#" class="comments">Comments (32)</a> &nbsp;&bull;&nbsp;&nbsp; <a href="#" class="more">Read full post &raquo;</a></p>
	        </div>
	         -->
	<%
	}//end if
    %>
<!-- ------------} -->        
<%
}
%>
<!-- sub_userinfo end -->
