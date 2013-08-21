<%@ page pageEncoding="utf-8" %>
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat"><a id="gossipReplyCount" href="http://www.${applicationScope.sysdomain}/myreplylist.do"></a></span></h2>
		<span class="more" id="replyMoreDiv"><a href="http://www.${applicationScope.sysdomain}/myreplylist.do">更多</a></span>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>	
<%@ include file="/pages/register/guide_news_feed.jsp"%><%--新鲜事 --%>  
<div class="find-friend">
	<h3><c:if test="${requestScope.guide_page=='guide_app'}">首先，</c:if>来找找认识的人吧！O(∩_∩)O~</h3>
	<span class="perfect-info">
		<a href="${applicationScope.urlWww}/BasicInfo.do?ref=guide_people_cp">更多信息=更多好友 &raquo;</a>
	</span>
	<%@ include file="/views/guide/xf/common/middle/guide_middle_search.jsp"%>	
		<c:if test="${!empty requestScope.companyAndSchool}">
		<c:forEach varStatus="status" var="csItem" items="${requestScope.companyAndSchool}">
			<c:if test="${csItem.count>0}">
				<div class="users">
			       	<h4>
			       		<c:choose>	
			       			<c:when test="${csItem.type=='location'}">
			       				${csItem.typeName}
			       			</c:when>
			       			<c:when test="${csItem.type=='work'}">
			       				<a href=${applicationScope.urlBrowse}/searchEx.do?from=guide&amp;${csItem.moreLink}>${csItem.typeName}</a> 
			       				的同事<span class="num">${csItem.count}人</span>
			       			</c:when>
			       			<c:otherwise>
								<a href=${applicationScope.urlBrowse}/searchEx.do?from=guide&amp;${csItem.moreLink}>${csItem.typeName}</a> 
			       				的同学<span class="num">${csItem.count}人</span>	
							</c:otherwise>
						</c:choose>			       		
			       	</h4>
					<ul class="clearfix" id="guids-uses-con${status.index}"></ul>
			    </div>
				<div id="showLessCon${status.index}" class="legend" style="display:none">
					<c:if test="${csItem.type!='location'}">
						<div class="desc">
							<a href=${applicationScope.urlBrowse}/searchEx.do?ref=guide_people_more_sr&amp;${csItem.moreLink} target="_blank">
								查看全部来自${csItem.typeName}(共${csItem.count}人) &raquo;
							</a>
						</div>
					</c:if>
					<a id="showLess${status.index}" class="toggle" href="javascript:void(0);">收起</a>
				</div>
				<div id="showMoreCon${status.index}" class="show-more" style="display:none">
					<a id="showMore${status.index}" href="javascript:void(0);" onclick="ajax_stat_link('more','guide_people_more');"><span>显示更多</span></a>
				</div>
			</c:if>
		</c:forEach>	
	</c:if>	
	<c:if test="${fn:length(sameIPList)>0}">				
		<div class="users">
	       	<h4>在你附近上网</h4>
			<ul class="clearfix" id="guids-uses-con99"></ul>
	    </div>
		<div id="showLessCon99" class="legend" style="display:none">
			<a id="showLess99" class="toggle" href="#guids-uses-con${status.index}">收起</a>
		</div>
		<div id="showMoreCon99" class="show-more" style="display:none">
			<a id="showMore99" href="javascript:void(0);" onclick="ajax_stat_link('more','guide_people_more');"><span>显示更多</span></a>
		</div>	
	</c:if>	
</div>
<c:if test="${requestScope.guide_page=='guide_app'}">
	<%@ include file="/views/guide/xf/common/middle/guide_hot_share.jsp"%>
</c:if>

                      	
