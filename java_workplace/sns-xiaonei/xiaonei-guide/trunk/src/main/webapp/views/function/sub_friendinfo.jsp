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
<%@page import="com.xiaonei.platform.component.friends.logic.HotFriendLogic"%><div></div>
<h2>Friend Info</h2>
<!-- sub_friendinfo -->

<%
//User host;
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null && MyStringUtils.equals("on",request.getParameter("friend"))){
%>

<!-- {------------ --> 

<ul>
    <%
    int checkfriend = request.getParameter("fid")==null||request.getParameter("fid").equals("")?0:(Integer)MyStringUtils.stringToObject(request.getParameter("fid"),Integer.class); 
    
    String q = "";//request.getQueryString();
    java.util.List<java.lang.Integer> friendIds = com.xiaonei.platform.component.friends.home.FriendsHome.getInstance().getFriendsIds(host.getId());
    List<Integer> idList = FriendsHome.getInstance().getApplyFriendList(host.getId());
    Map<Integer, WUserCache> ucs = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(idList);
    List<com.xiaonei.platform.component.friends.view.FriendListView> specialList = HotFriendLogic.getInstance().getHotFriendList(host.getId());
    String checkret = "Not Friend";
    if(friendIds.contains(checkfriend)){
    	checkret = "<font color=green>Friend</font>";
    }
    for(com.xiaonei.platform.component.friends.view.FriendListView v : specialList){
	    if(v!=null && v.getId() == checkfriend){
	    	checkret = "<font color=red>Special Friend</font>";
	    }
    }
    
    List<Integer> commanList = new ArrayList<Integer>();
    if(checkfriend!=0){
	    int commanFriendCount = 0;
	    //host.getId() and checkfriend
	    List<Integer> idListOfB = FriendsHome.getInstance().getFriendsIds(checkfriend);
	    //idList and idListOfB
	    for(int idofB : idListOfB){
	    	if(friendIds.contains(idofB)){
	    		commanList.add(idofB);
	    	}
	    }
    }
    
    
    out.println("<li>Check "+checkfriend+":"+checkret+"</li>");
    out.println("<li>Common Friend "+":"+commanList.size()+"<br />");
    int commonindex = 0;
    for(int idofB :commanList){
	    out.println(commonindex+++""+":"+idofB+"<br />");
    }
    out.println("</li>");
    out.println("<li>Friend Count:"+friendIds.size()+"</li>");
    out.println("<li>Request Friend Count:"+idList.size()+"</li>");
    out.println("<li>Special Friend Count:"+specialList.size()+"");
    out.println("");
    for(int i=0;i<specialList.size();i++){    
    	com.xiaonei.platform.component.friends.view.FriendListView f=specialList.get(i);
        if(f!=null){
            out.println("Special:"+f.getId()+"["+f.getName()+"]<br />");   
        }         
    }  
    out.println("</li>");
    String friend = request.getParameter("friend");
    if(MyStringUtils.equals("on",friend)){
    	Map<Integer, WUserCache> friendcache = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(friendIds);
    	int i = 0;
    	out.println("<li>");
    	for(int fid:friendIds){
    		WUserCache cat = friendcache.get(fid);
    		if(cat!=null){
    		    out.println(" "+i+++":"+fid+"["+cat.getName()+"]("+cat.getStatus()+")<br />");   
    		}
    	}
    	out.println("</li>");
    }
    out.println("<li>");
    for(int i=0;i<idList.size();i++){    
        WUserCache wuc=ucs.get(idList.get(i));
        if(wuc!=null){
            out.println("SendTo:"+wuc.getId()+"["+wuc.getName()+"]<br />");   
        }         
    }  
    out.println("</li>");
    %>
    <%
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