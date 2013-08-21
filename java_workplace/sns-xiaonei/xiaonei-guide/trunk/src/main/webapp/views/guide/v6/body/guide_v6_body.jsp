<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@page import="com.xiaonei.reg.guide.pojo.RecommendContent"%>
<%@page import="java.util.List"%>

<%! 
List<RecommendContent> recommendContentList=null;
%>

<%
recommendContentList=(List<RecommendContent>)request.getAttribute("recommendcontentlist");
%>

<%
   if(recommendContentList!=null && recommendContentList.size()>0){
   
%>

<div id="suggestDiv">
		<div class="header clearfix">
			<h4>热门推荐</h4>
			<a href="javascript:;" id="suggestChange">换一换</a>
		</div>
		<ul id="suggestList" style="margin-left:0;">
			<c:forEach var="recommend" items="<%=recommendContentList%>">
				<li id="box">
					<div class="box">
						<img src="${recommend.siteInfo.head}" width="115"/>
						<div id="on" style="top:92px;">
							<div class="detail">
								<a href="${recommend.siteInfo.url}" target="_blank"><span>${recommend.siteInfo.name}</span></a>
								<p>${recommend.siteInfo.description}</p>
							</div>
						</div>
					</div>
					<a href="javascript:;" data-id="${recommend.id}" data-businessid="${recommend.siteInfo.id}" data-tag="${recommend.interest}" class="suggest" id="like">关注</a>
				</li>
			</c:forEach>
		</ul>
	</div>

<%
}
%>

<!-- 新鲜事部分 -->
<div class="guide-tabs-content" id="guideTabCon1">
    <%--新鲜不再由后台输出，ajax获取 By bo.hu@opi-corp.com 2010年05月31日 星期一 17时39分29秒
        <%@ include file="/views/guide/xf/zottf/middle/guide_news_feed.jsp"%>
    --%>
	<div class="feed-new-title">
		<div class="fliter-box">
			<strong><span>新鲜事</span></strong>
		</div>
	</div>
    <div class="news-feed">        
        <div class="feed-holder" id="feedHolder">
            <jsp:include page="/inc/n/modules/newsfeed.jsp">
            	<jsp:param name="canReadAll" value="false" />   
        		<jsp:param name="loadOnScroll" value="true" />      		
    		</jsp:include>		
        </div>
        <%--新鲜事需要用的数据--%>
        <input type="hidden" id="userIdFP" value="${visiter.id}" />
    </div>
</div><!-- 新鲜事结束 -->

<%@ include file="/views/guide/xf/zot/body/guide_xf_v6_body.jsp"%>




