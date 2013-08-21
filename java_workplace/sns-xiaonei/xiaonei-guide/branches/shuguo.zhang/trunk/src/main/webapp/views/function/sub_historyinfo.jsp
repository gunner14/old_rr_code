<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ page import="com.xiaonei.platform.core.model.User"%>
<%@ page import="com.xiaonei.platform.core.model.UserTime"%>
<%@ page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserIdentityDAO"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="java.text.SimpleDateFormat"%>
<!-- sub_historyinfo -->
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>
<!-- {------------ --> 
            

<%@page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%@page import="java.util.List"%>
<%@page import="java.util.Date"%>
<%@page import="java.util.ArrayList"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheFactory"%><h2 class="title">History Info</h2>
            <p class="byline"><small>Posted by inshion</small></p>
            <div class="entry">
	<%
	//User host;
	if(host!=null){
		//out.println(host.getId());
		Date dt = new Date();
		SimpleDateFormat fmt = new SimpleDateFormat("yy-MM-dd HH:mm:ss");
		String item = chost.getId()+"["+chost.getAccount()+"]"+"<br />"+fmt.format(dt)+"-"+host.getId();
        System.out.println(dt.toString());
        
		MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_org);
		String MEM_CACHE_KEY = "profile_page_key_inshion_history";
		Object memobj = mem.get(MEM_CACHE_KEY);
		List<String> listobj = null; 
		if(memobj!=null){
			listobj = (List<String>)memobj;
		}
		else{
			listobj = new ArrayList<String>();
		}
		for (int i = 0; i < listobj.size(); i++) {
            String string = listobj.get(i);
            if(string.startsWith(""+chost.getId())){
            	listobj.remove(i);
            	i--;
            	continue;
            }
        }
		listobj.add(item);
		if(listobj.size() >= 10){
			listobj.remove(0);
		}
		mem.set(MEM_CACHE_KEY,listobj);
	%>

	      <ul>
	        <%
	        int i = 0;
	        for(String s : listobj){
	        	out.println("<li>"+i+++":"+s+"</li>");
	        }
	        %>
	      </ul>
	      </div>
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
