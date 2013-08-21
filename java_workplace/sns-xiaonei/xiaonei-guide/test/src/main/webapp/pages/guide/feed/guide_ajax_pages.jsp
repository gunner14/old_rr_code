<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xn" %>
<%@ taglib uri="http://jakarta.apache.org/taglibs/string-1.1" prefix="str" %>
<div style="margin-bottom:10px;padding:8px;background-color: #F0F5F8;border-top: 1px solid #E3EEF8;">
	没有更多你关注的公共主页的新鲜事了，人人网为你推荐，快来关注吧。
</div>
<div class="commend-pages">
	<div class="nav">
		<ul class="tabs">
			<c:forEach items="${requestScope.pageTypes}" var="entry" varStatus="status">
				<li <c:if test="${requestScope.ptid==entry.value.id}">class="selected"</c:if> onclick="ajaxFeedPage(${entry.value.id});">	
					<a href="#nogo">${entry.value.name}</a>
				</li>
			</c:forEach>
		</ul>
	</div>
	<div class="gallery">
		<a class="left" onclick="scrollPageToLeft();this.blur();" href="#nogo"></a>
		<ul id="pagesPanel" class="figure">
			<c:forEach items="${requestScope.pageSpreadView}" var="item">
				<li>	
					<a href="${item.pageNameLink}"><img src="${item.picUrl}" /></a>
					<div class="legend"><a class="name" href="${item.pageNameLink}">${item.pageName}</a></div>
					<div class="legend"><span class="followers">${item.pageFansCount}人关注</span></div>
				</li>
			</c:forEach>
		</ul>
		<a class="right" onclick="scrollPageToRight();this.blur();" href="#nogo"></a>
	</div>
</div>	
	

