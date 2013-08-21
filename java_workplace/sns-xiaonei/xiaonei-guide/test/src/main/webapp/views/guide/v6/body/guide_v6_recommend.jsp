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


<c:forEach var="recommend" items="<%=recommendContentList%>">
	<li id="box">
		<div class="box">
			<img alt="${recommend.siteInfo.description}" src="${recommend.siteInfo.head}" width="115"/>
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