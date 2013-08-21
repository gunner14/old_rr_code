<%@page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="com.xiaonei.platform.core.model.User"%>
<%@page import="com.xiaonei.platform.core.model.UserTime"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserIdentityDAO"%>
<%@page import="com.xiaonei.reg.guide.root.miscs.MyStringUtils"%>
<%@page import="com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserDecisionLogic"%>
<%@page import="com.xiaonei.reg.guide.root.logics.StepProcessLogic"%>
<%@page import="com.xiaonei.reg.register.model.RegExtraMsg"%>
<%@page import="com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic"%>
<%@page import="java.util.Map"%>
<%@page import="com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic"%>
<%@page import="com.xiaonei.reg.guide.dao.GuideUserDecisionDAO"%>
<%@page import="java.util.List"%>
<%@page import="com.xiaonei.reg.guide.logic.StepStatusLogic"%>

<!-- sub_guideinfo -->
<%
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>
<!-- {------------ --> 
            
<%@page import="java.util.Date"%>
<%@page import="com.xiaonei.xce.scorecache.ScoreCacheAdapter"%>
<%@page import="com.xiaonei.xce.feed.FeedCacheAdapter"%>

<%@page import="com.xiaonei.xce.feed.FeedLogicAdapter"%>
<%@page import="com.xiaonei.reg.register.dao.UserRegisterIpDAO"%>
<h2 class="title">Register Guide Info</h2>
            <p class="byline"><small>Posted by inshion</small></p>
            <div class="entry" id="guideinfo_div">

	<%
	//User host;
	if(host!=null){
		//out.println(host.getId());
	    String froms = RegFromSourceLogic.getFromSource(host);
	    int utype = StepProcessLogic.getInstance().getGuideUserType(host,froms);
	    int dec = GuideXFiveUserDecisionLogic.getInstance().getDecision(host.getId());
	    int decinDao = GuideUserDecisionDAO.getInstance().getDecision(host.getId());
	    boolean isguide = host.getUserState().isGuideDone();
	    
	    Map<String,RegExtraMsg> map =  RegExtraMsgLogic.getInstance().loadExtraMsgMap(host);
	    int extraindex = 0;
	%>
                <p>
                Below is his/her register info.
                </p>
                <ul>
                    <%
				    for (String k : map.keySet()) {
				        out.println(" reg_extra:"+(extraindex++)+" ["+map.get(k).getInput_key()+","+map.get(k).getInput_value()+"] <br />");
				    }
                    int step = StepStatusLogic.getStepStatus(host.getId());
                    %>
                    <li>Register From:[<%=froms %>]</li>
                    <li>Guide User_Type:<%=utype %></li>
                    <li>Guide User_Decision:<%=dec %>[<%=decinDao %>]</li>
                    <li>Is Home User:<%=isguide %>[<%=step %>]</li>
                    
                    <%
                    out.println("<li>");
                    List<String> list = StepStatusLogic.doTest(step);
                    for(String s : list){
                        out.println(" "+s+" <br />");
                    }
                    out.println(" </li>");
                    %>
                    
                    <%
                    try {
                        UserTime hosttime = SnsAdapterFactory.getUserTimeAdapter().getUserTime(host.getId());
                        
                        Date regtime = hosttime.getRegisterTime();
                        Date currenttime = new Date();
                        long datediff = currenttime.getTime() - regtime.getTime();
                        out.println("<li> Register Days:"+datediff / (1000*60*60*24) +" </li>");
                        Date acttime = hosttime.getActivateTime();
                        long actday = (currenttime.getTime() - acttime.getTime()) / (1000*60*60*24);
                        out.println("<li> Actived Days:"+actday +" </li>");
                        
                        int feedcount = FeedCacheAdapter.getInstance().getCount(host.getId());
                        out.println("<li> News Feed:"+feedcount+" "+" </li>");
                        
                        boolean hasmini = FeedLogicAdapter.getInstance().hasMiniFeed(host.getId());
                        out.println("<li> Mini Feed:"+hasmini+" "+" </li>");
                        
                        String reg_ip = UserRegisterIpDAO.getInstance().getUserRegisterIp(host.getId());
                        request.setAttribute("reg_ip",reg_ip);
                        out.println("<li> Register Ip:<a href='#' onmouseover='showAddress();' onmouseout='hideAddress();'>"+reg_ip+"</a> "+"</li>");
                        out.println("<div style='display:none;background:#FFFBC1;position:absolute;border:1px solid #F9B967;padding:5px;' id='ajax_show_ip'></div>");
                        //<a href='http://www.ip.cn/getip.php?action=queryip&ip_url="+reg_ip+"'></a> 
                        
                        
                    } catch (Exception e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
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
<script type="text/javascript">
var addtarget = "";
	function showAddress(){
		var x = 0;
        
		if(addtarget == ""){
			new XN.net.xmlhttp({
	            url:'http://guide.renren.com/ajaxGuideGetUrl',
	            data:'ip_url=<%=request.getAttribute("reg_ip")%>',
	            onSuccess:function(r){
	            	addtarget = r.responseText;
	            	doShow(addtarget);
	            	
	            }
	        });
		}
		else{
			doShow(addtarget);
		}
		
		
	}

	var cx ;
	var cy ;
	function mouseMove(ev)
	{
		ev= ev || window.event;
		var mousePos = mouseCoords(ev);
		//alert(ev.pageX);
		//document.getElementById("xxx").value = mousePos.x;
		//document.getElementById("yyy").value = mousePos.y;
		cx = mousePos.x;
		cy = mousePos.y;
	}

	function mouseCoords(ev)
		{
		if(ev.pageX || ev.pageY){
		return {x:ev.pageX, y:ev.pageY};
		}
		return {
		x:ev.clientX + document.body.scrollLeft - document.body.clientLeft,
		y:ev.clientY + document.body.scrollTop - document.body.clientTop
		};
	}

	document.onmousemove = mouseMove; 

	function doShow(text){
		$('ajax_show_ip').innerHTML = text;
        $('ajax_show_ip').style.display='block';
        $('ajax_show_ip').style.left=cx+'px';
	}
	function hideAddress(){
		$('ajax_show_ip').innerHTML = '';
		$('ajax_show_ip').style.display='none';
	}

	
</script>