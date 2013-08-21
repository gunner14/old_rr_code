<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.platform.component.friends.home.FriendsHome"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>

<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.platform.core.usercount.UserCountMgr"%>
<%@page import="java.util.Properties"%>
<%@page import="java.io.IOException"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Map.Entry"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%>
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheWithTotalN"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheDataN"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%>
<%@page import="com.xiaonei.platform.core.model.UserState"%>
<%@page import="com.xiaonei.platform.core.model.UserConfigInfo"%><div></div>
<h2>State Info</h2>
<!-- sub_stateinfo -->

<%
//User host;
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<ul>
    <%
    String q = "";//request.getQueryString();
    String dostar = ""; 
    String img_true = "<img src=\"images/star_true.png\" />";
    String img_false = "<img src=\"images/star_false.png\" />";
   	if(!MyStringUtils.contains(q,"id="+host.getId())){
   		q = q+"&id="+host.getId()+"&run=true";
   	}
    if(host.isStarUser()){
    	q = q+"&action=removestar";
    	dostar = "remove";
    }
    else{
    	q = q+"&action=setstar";
        dostar = "setstar";
    }
    
    java.util.List<java.lang.Integer> friendIds = com.xiaonei.platform.component.friends.home.FriendsHome.getInstance().getFriendsIds(host.getId());
    boolean isvip = host.getUserState().isVipMember();
    out.println("<li>Viped :"+(isvip)+" ");
    out.println("<li>Stared:"+(host.isStarUser()?img_true:img_false)+" ");
    out.println("<a href=\"procont.jsp?"+q+"\">["+dostar+"]</a></li>");
    out.println("<li> Passport State:"+ host.getUserPassportInfo().getStatus() +" </li>");
    UserState us = host.getUserState();
    out.println("<li> User State:Lv."+us.getLevel() +" St."+us.getState()+" </li>");
    
    out.println("<li> -1=仅我 0=好友 3=同城 99=所有<br />");
    try{
        UserConfigInfo cinfo = SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(host.getId());
    	int priv1 = cinfo.getBasicConfig();
        int priv2 = cinfo.getPersonalValue();
        int priv3 = cinfo.getEducationValue();
        int priv4 = cinfo.getWorkValue();
        int priv5 = cinfo.getGossipValue();
        int priv6 = cinfo.getMessageConfig();
        //int priv7 = cinfo.getQqValue();
        int priv8 = cinfo.getMsnValue();
        int priv9 = cinfo.getMobilelValue();
        int priv10 = cinfo.getBrowseConfig();
        String priv11 = cinfo.getProfilePrivacy();
        
        
        out.println("基　本:"+priv1+" | ");
        out.println("个　人:"+priv2+"<br />");
        out.println("学　校:"+priv3+" | ");
        out.println("工　作:"+priv4+"<br />");
        out.println("留言框:"+priv5+" | ");
        out.println("留　言:"+priv6+"<br />");
        out.println("口　口:"+"n"+" | ");
        out.println("摸死你:"+priv8+"<br />");
        out.println("手　机:"+priv9+" | ");
        //out.println("Profile:"+priv10+"");
        out.println("<!-- Profile:"+priv11+"--> </li>");
    }
    catch(Exception e){
    	System.err.print("sub_stateinfo:"+e);
    }
    
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
<!-- {------------ --> 
<%
}
%>