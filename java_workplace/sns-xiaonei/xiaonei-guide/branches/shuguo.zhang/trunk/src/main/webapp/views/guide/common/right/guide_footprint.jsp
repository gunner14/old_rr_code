<%@page pageEncoding="UTF-8" %>
<%@page import="com.xiaonei.reg.guide.model.SnsFootprintView"%>
<%
	java.util.List<SnsFootprintView> visitList = (java.util.ArrayList<SnsFootprintView>)request.getAttribute("visitList");
	if(visitList != null && visitList.size() > 0) {
	    if(visitList.size()>=3){
	        visitList=visitList.subList(0,3);
	    }
%>
<div class="side-item footprint with-arrow" id="footPrint">
	<div class="side-item-header clearfix">
		<h4>最近来访<span> (${requestScope.viewcount})</span></h4>
	</div>
	<div class="side-item-body clearfix">
		<div style="width:195px;overflow:hidden;margin-top:-40px;padding-top:40px;*position:relative;">
			<ul class="people-list" id="footPrintList" style="margin-left:0px;">
	<%
			for(SnsFootprintView view:visitList) {
				try {
					boolean isOnline=view.isOnline();
					boolean isWapOnline=view.isWapOnline();
					boolean isVip=view.isVip();
					boolean isKeepUse = view.isKeepUse();
	%>
				<li<%if(isOnline || isWapOnline) {%> class="online"<%}%> onmouseover="$(this).addClass('fphover');" onmouseout="$(this).delClass('fphover')">
					<span class="headpichold">
						<a style="cursor:pointer;" namecard="<%=view.getGuest()%>" href="${applicationScope.urlMain}/profile.do?portal=homeFootprint&amp;ref=home_footprint&amp;id=<%=view.getGuest()%>"><img src="<%out.print(com.xiaonei.platform.core.head.HeadUrlUtil.getHeadFullUrl(view.getGuestUrl()));%>" onload="roundify(this)" /></a>
					</span>
					<span class="<%if(isOnline || isWapOnline){%>olname <%}%>clearfix">
						<%if(isVip && isOnline){%>
							<img class="on-line" src="${applicationScope.urlStatic}/imgpro/icons/vip-online.gif" width="13" height="12" title="VIP在线,点此和<%=view.getGuestName()%>聊天" onclick="javascript:talkto(<%=view.getGuest()%>, '<%=view.getGuestName()%>');return false;" />
						<%}else if(isOnline){%>
							<img class="on-line" src="${applicationScope.urlStatic}/imgpro/icons/online_1.gif" width="13" height="12" title="点此和<%=view.getGuestName()%>聊天" onclick="javascript:talkto(<%=view.getGuest()%>, '<%=view.getGuestName()%>');return false;" />
						<%}else if(isWapOnline){%>	
							<c:choose>
								<c:when test="${domainIdentified['sys.domain']=='kaixin.com'}">
									<img class="wap-on-line" src="${applicationScope.urlStatic}/imgpro/icons/wap-online.gif" width="13" height="12" title="当前手机${domainIdentified['fullSiteName']}在线" onclick="window.open('http://mobile.${applicationScope.sysdomain}/kxhome?psf=40009')"/>
								</c:when>
								<c:otherwise>
									<img class="wap-on-line" src="${applicationScope.urlStatic}/imgpro/icons/wap-online.gif" width="13" height="12" title="当前手机${domainIdentified['fullSiteName']}在线" onclick="window.open('http://mobile.${applicationScope.sysdomain}/home?psf=40010')"/>
								</c:otherwise>
							</c:choose>							
						<%}%>
						<a <c:if test="<%=isKeepUse %>">class="lively-user" title="连续登录7天, 即可获得橙名特权"</c:if> href="${applicationScope.urlMain}/profile.do?portal=homeFootprint&amp;ref=home_footprint&amp;id=<%=view.getGuest()%>" namecard="<%=view.getGuest()%>"><%=view.getGuestName()%></a>
					</span>
					<span class="time-tip"><% out.print(view.getDateTipString()); %></span>
				</li>
	<%
				} catch(Exception e) {
					//never happening
					e.printStackTrace();
				}
			}
	%>
			</ul>
		</div>
		
		<a class="foot-forward foot-forward-disable" id="footForward" href="javascript:;" title="下一页" onclick="slideShow.getMore();">下一页</a>
		<a class="foot-back foot-back-disable" id="footBack" href="javascript:;" title="" onclick="slideShow.getBack();" style="cursor:default">上一页</a>
		<a style="position:absolute;top:17px;*top:-22px;_top:-23px;right:0;" href="${applicationScope.urlMain}/myfoot.do">更多</a>		
		<c:if test="${show_home_right_footprint_new_act_tip && (host.id%10 ==6)}">
			<div style="padding:5px;margin-top:-10px;margin-bottom:5px;border:1px solid #CEE1EE;background-color:#F3FAFF;*margin-top:-5px;">
				<p style="color:#666;"><span style="color:#F90">新！</span>访问你相册和日志的好友会在最近来访留下脚印，同时增加你的人气。</p>
			</div>
		</c:if>
		
	</div>
	<input type="hidden" value="${requestScope.viewcount}" id="visitCount" />
	<input type="hidden" value="${host.id}" id="userIdFP" />
</div>
<%
	}
%>
