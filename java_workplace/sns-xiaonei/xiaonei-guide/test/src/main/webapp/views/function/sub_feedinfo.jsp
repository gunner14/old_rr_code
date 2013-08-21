<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_feedinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="com.xiaonei.platform.component.xfeed.display.FeedFactory"%>
<%@page import="com.xiaonei.platform.component.xfeed.display.IFeedRetriever"%>
<%@page import="java.util.Map"%>
<%@page import="java.util.Map.Entry"%>
<div></div>
<%
//User host;
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Feed Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
<div class="feed-list">
    <%
    if(host.getId() == 81120 && false){
    %>
	<%
	   FeedFactory  factory= FeedFactory.getInstance();
	   Map<String, Object> miniFeedMap = factory.showMini(host.getId(), host.getName(), host.getId(), 0, 10);
	   if (miniFeedMap != null && miniFeedMap.size() > 0) {
	       for(String key : miniFeedMap.keySet()){
	    	   out.println(key+":"+miniFeedMap.get(key));
	       }
	   } else {
	   }
	   
	%>
	 	    
    <%
    }
    %>
    <%
    String feed = request.getParameter("feed");
    if(MyStringUtils.equals("on",feed)){
	    //List<String> list=FeedViewAdapter.getInstance().getLive(host.getId(),host.getName(),0, 0, IFeedRetriever.HOME_LIMIT);
	    Map<String, ? super Object> map = FeedFactory.getInstance().showNews(host.getId(), host.getName(), 0,IFeedRetriever.HOME_LIMIT);
	    if (map != null) {
	        StringBuffer feedString = new StringBuffer();
	        /*
	        for (int i = 0; i < map.size(); i++) {
	            feedString.append(map);
	        }
	        */
	        /*
	        for(Map.Entry<String, ? super Object> ent : map.entrySet()){
	        	feedString.append(ent.getKey()+":"+ent.getValue()+"<br />");
	        }
	        */
	        out.println(map.get("content"));
	    }
	    else{
	        out.println("无新鲜事");
	    }
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</div>
</ul>
</div>
<!-- {------------ --> 
<%
}
%>