<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_minifeedinfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>

<%@page import="com.xiaonei.platform.component.xfeed.display.FeedFactory"%><div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">MiniFeed Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
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
    String minifeed = request.getParameter("minifeed");
    if(MyStringUtils.equals("on",minifeed)){
	    FeedFactory  factory= FeedFactory.getInstance();
	    Map<String, Object> miniFeedMap = factory.showMini(host.getId(), host.getName(), host.getId(), 0, 10);
	    if (miniFeedMap != null && miniFeedMap.size() > 0) {
	    	out.println(miniFeedMap.get("content"));
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