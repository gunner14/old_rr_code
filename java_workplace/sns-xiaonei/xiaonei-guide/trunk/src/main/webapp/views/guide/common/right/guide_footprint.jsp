<%@page pageEncoding="UTF-8" %>
<%@page import="com.xiaonei.reg.guide.model.SnsFootprintView"%>
<%
	java.util.List<SnsFootprintView> visitList = (java.util.ArrayList<SnsFootprintView>)request.getAttribute("visitList");
	if(visitList != null && visitList.size() > 0) {
%>
<div class="side-item footprint with-arrow" id="footPrint">
	<div class="side-item-header clearfix">
		<h4>最近来访<span> ${requestScope.viewcount}</span></h4>
	</div>
	<div class="side-item-body clearfix">
		<div id="people-content" style="margin-top:-40px;padding-top:40px;*position:relative;width:240px;z-index:3;">
			<ul class="people-list" id="footPrintList" style="margin-left:0px;width:10000px">
	<%
	        int size=visitList.size();
			SnsFootprintView view=null;
			for(int i=0;i<size;i++) {
				try {
				    view=visitList.get(i);
					boolean isOnline=view.isOnline();
					boolean isWapOnline=view.isWapOnline();
					boolean isVip=view.isVip();
					boolean isKeepUse = view.isKeepUse();
	%>
				<li onmouseover="$(this).addClass('fphover');$('footForward').style.zIndex='';$('footBack').style.zIndex='';$('moreFooter').style.zIndex='';" onmouseout="$(this).delClass('fphover');$('footForward').style.zIndex='3';$('footBack').style.zIndex='3';$('moreFooter').style.zIndex='3';">
					<span class="headpichold">
						<a style="cursor:pointer;" namecard="<%=view.getGuest()%>" href="${applicationScope.urlMain}/profile.do?portal=homeFootprint&amp;ref=home_footprint&amp;id=<%=view.getGuest()%>"><img src="<%out.print(com.xiaonei.platform.core.head.HeadUrlUtil.getHeadFullUrl(view.getGuestUrl()));%>" width="30" height="30" /></a>
					</span>
					<%
					    if(i==0){
					%>
					<span class="time-tip first-tip">
					<%
					  }
					  else if(i==5){
					%>
					<span class="time-tip last-second-tip">
					<%
					   }
					   else if(i==6){
					%>
					<span class="time-tip last-tip">
					<%
						}
						else{
					%>
					<span class="time-tip">
					<%
						}
					%>
						<span class="tip-content"><%=view.getGuestName()%> <% out.print(view.getDateTipString()); %></span>
						<span class="tip-footer"></span>
					</span>
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
		<c:choose>
			<c:when test="${timeStamp>=20120322 && timeStamp<=20120410}">
				<p style="padding-bottom:5px;"><a style="display:inline-block;zoom:1;background:url(http://a.xnimg.cn/imgpro/icons/hi.png) left center no-repeat;padding-left:22px;padding-bottom:5px;" href="http://i.renren.com/act/friendback?wc=20120322" target="_blank">召唤好友登录人人网，获取VIP会员</a></p>
			</c:when>
			<c:otherwise>
				<c:if test="${show_home_right_footprint_new_act_tip && (host.id%10 ==6)}">
					<div style="padding:5px;margin-top:-10px;margin-bottom:5px;border:1px solid #CEE1EE;background-color:#F3FAFF;*margin-top:-5px;">
						<p style="color:#666;"><span style="color:#F90">新！</span>访问你相册和日志的好友会在最近来访留下脚印，同时增加你的人气。</p>
					</div>
				</c:if>
			</c:otherwise>
		</c:choose>
		
		<a onclick="slideShow.getMore();return false;" title="下一页" href="#" id="footForward" class="foot-forward foot-forward-disable" style="z-index:3">下一页</a>
		<a style="cursor:default;z-index:3" onclick="slideShow.getBack();return false;" title="" href="#"  id="footBack" class="foot-back foot-back-disable">上一页</a>
		<a id="moreFooter" title="更多" href="http://www.renren.com/myfoot.do" style="z-index:3;position:absolute;top:17px;*top:-22px;_top:-23px;right:0;" class="more">更多</a>
	</div>
	<input type="hidden" value="${requestScope.viewcount}" id="visitCount" />
	<input type="hidden" value="${host.id}" id="userIdFP" />
</div>
<%
	}
%>
