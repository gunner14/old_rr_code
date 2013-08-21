<%@ page contentType="text/html;charset=UTF-8" language="java" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<div class="wiki-liner"></div>
<div class="wiki-intro-avatar">
	<a href="http://wiki.renren.com/${wiki.id}"><img src="${wiki.mainUrl}" alt="${wiki.name}" width="130" /></a>
</div>
<div class="wiki-intro-oper-box">
	<h1 class="wiki-intro-title">${wiki.name}</h1>
	<span class="wiki-score" title="评分为${valuationDisplayChars}分"<c:if test="${valuationDisplayChars eq '0'}"> style="display:none"</c:if>>${valuationDisplayChars}</span>
	<div class="wiki-intro-oper clearfix">
		<c:choose>
			<c:when test="${isFollowing}">
				<a class="wiki-liked-btn" href="javascript:void(0)" wikibtn="${wiki.id}" data-do="like" data-done="1">喜欢</a>
			</c:when>
			<c:otherwise>
				<a class="wiki-like-btn" href="javascript:void(0)" wikibtn="${wiki.id}" data-do="like" data-done="0">喜欢</a>
			</c:otherwise>
		</c:choose>
		<div class="w-btnbox">
			<a id="wiki_btn_do_0" href="javascript:void(0)" style="display:none;" class="w-gray-btn left-btn <c:if test="${userActionRecord.interest eq 'wish' }"> w-choose-btn</c:if>" data-done="${userActionRecord.interest eq 'wish'?1:0 }" data-do="wish" wiki-btn="${wiki.id}">想看</a>
			<a id="wiki_btn_do_1" href="javascript:void(0)" style="display:none;" class="w-gray-btn left-btn <c:if test="${userActionRecord.interest eq 'do' }">w-choose-btn</c:if>" data-done="${userActionRecord.interest eq 'do'?1:0 }" data-do="do" wiki-btn="${wiki.id}">在看</a>
			<!--选中状态需要给class增加样式w-choose-btn-->
			<a id="wiki_btn_do_2" href="javascript:void(0)" style="display:none;" class="w-gray-btn right-btn <c:if test="${userActionRecord.interest eq 'collect' }">w-choose-btn</c:if>" data-done="${userActionRecord.interest eq 'collect'?1:0 }" data-do="collect" wiki-btn="${wiki.id}">看过</a>	
		</div>
		<!--<div class="w-btnbox">
			<a href="javascript:void(0)" class="w-gray-btn w-done-btn">已看过</a>	
		</div>-->
		
		<div style="display:none;" class="wiki-stars">
			<span>评分：</span>
            <span data-rate="${userActionRecord.rating}" class="wiki-stars-rate${userActionRecord.rating} wiki-stars-rate"><a></a><a></a><a></a><a></a><a></a></span>
			<span class="wiki-stars-txt"></span>
		</div>
		
	</div>
</div>
