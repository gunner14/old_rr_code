<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.util.List"%>
<!-- sub_bloginfo -->

<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.xce.feed.FeedViewAdapter"%>
<%@page import="com.xiaonei.ugc.ubb.UBBLogic"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%>
<%@page import="com.xiaonei.blog.home.BlogHome"%>
<div></div>
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<h2 class="title">Blog Info</h2>
<p class="byline"><small>Posted by inshion</small></p>
<div class="entry">
<ul>
    <%
    String blog = request.getParameter("blog");
    String blog_s = request.getParameter("blog_s");
    String blog_bid = request.getParameter("blog_bid");
    if(MyStringUtils.equals("on",blog)){
    	out.println("-1:仅自己 4:加密 99:公开");
    	if(chost !=null && isMan(chost.getId())){
    		int nblog_s = (blog_s == null)?0:(Integer)MyStringUtils.stringToObject(blog_s,Integer.class);
    		int nblog_bid = MyStringUtils.stringToInt(blog_bid);
            List<com.xiaonei.blog.model.Entry> blogList = BlogHome.getInstance().getBlogListForProfile(host, host.getId(), -1, nblog_s, 10);
            int i = 0;
            for(com.xiaonei.blog.model.Entry en : blogList){
                out.println("<li>");
                out.println(i+++":"+"<font color=gray>["+en.getId()+"]"+"</font>"+en.getTitle()+"<font color=gray>["+en.getPasswordProtedted()+","+en.getBlogControl()+":"+en.getPassword()+"]"+"</font>"+en.getTime()+"<br />");
                if(nblog_bid == en.getId()){
                    out.println("<span style='background:#F3FAFF'>"+en.getBody()+"</span>");
                }
                out.println("</li>");
            }
        }
        else{
            List<com.xiaonei.blog.model.Entry> blogList = BlogHome.getInstance().getBlogListForProfile(host, host.getId(), -1, 0, 5);
            int i = 0;
            for(com.xiaonei.blog.model.Entry en : blogList){
                out.println("<li>");
                out.println(i+++":"+en.getTitle()+"["+en.getPasswordProtedted()+"]");
                out.println("</li>");
            }
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