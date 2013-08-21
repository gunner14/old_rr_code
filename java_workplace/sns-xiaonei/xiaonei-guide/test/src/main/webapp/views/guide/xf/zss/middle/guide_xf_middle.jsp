<%@ page pageEncoding="utf-8" %>
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat"><a id="gossipReplyCount" href="${applicationScope.urlWww}/myreplylist.do"></a></span></h2>
		<span class="more" id="replyMoreDiv"><a href="${applicationScope.urlWww}/myreplylist.do">更多</a></span>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>	
<%@ include file="/pages/register/guide_news_feed.jsp"%><%--新鲜事 --%>
<div class="find-friend-box">
	<h3><c:if test="${requestScope.guide_social=='social_bad'}">首先，</c:if>来找找认识的人吧！O(∩_∩)O~</h3>
	<span class="perfect-info">
		<a href="${applicationScope.urlWww}/profile.do?id=${requestScope.visiter.id}&amp;v=info&amp;act=edit&amp;ref=guide_people_cp">更多信息=更多好友 &raquo;</a>
	</span>		
	<%@ include file="/views/guide/xf/common/middle/guide_middle_search.jsp"%>	
	<c:if test="${!empty requestScope.companyAndSchool}">	
		<c:forEach varStatus="status" var="csItem" items="${requestScope.companyAndSchool}">
			<c:choose>
				<c:when test="${requestScope.guide_social=='social_bad'}">
					<c:if test="${csItem.count>0 && csItem.type!='location'}">				
						<div class="users" id="showUserItemCon${status.index}">
					       	<h4>
					       		<c:choose>					       			
					       			<c:when test="${csItem.type=='work'}">
					       				<a href=${applicationScope.urlBrowse}/searchEx.do?ref=sg_guide_${csItem.type}_search&amp;${csItem.moreLink} target="_blank">${csItem.typeName}</a> 
					       				的同事<span class="num">${csItem.count}人</span>
					       			</c:when>
					       			<c:otherwise>
										<a href=${applicationScope.urlBrowse}/searchEx.do?ref=sg_guide_${csItem.type}_search&amp;${csItem.moreLink} target="_blank">${csItem.typeName}</a> 
					       				的同学<span class="num">${csItem.count}人</span>	
									</c:otherwise>
								</c:choose>	
					       	</h4>
							<ul class="clearfix" id="guids-uses-con${status.index}"></ul>
					    </div>
						<div id="showLessCon${status.index}" class="show-more">
							<a href=${applicationScope.urlBrowse}/searchEx.do?ref=sg_guide_${csItem.type}_search&amp;${csItem.moreLink} target="_blank">
								显示更多&raquo;
							</a>						
						</div>					
					</c:if>	
				</c:when>
				<c:when test="${requestScope.guide_social=='social_good' || requestScope.guide_page=='guide_netbarxfive' || requestScope.guide_page=='guide_APPMZMG'}">
					<c:if test="${csItem.count>0}">				
						<div class="users">
					       	<h4>
					       		<c:choose>	
					       			<c:when test="${csItem.type=='location'}">
					       				${csItem.typeName}
					       			</c:when>
					       			<c:when test="${csItem.type=='work'}">
					       				<a href=${applicationScope.urlBrowse}/searchEx.do?ref=sg_guide_${csItem.type}_search&amp;${csItem.moreLink} target="_blank">${csItem.typeName}</a> 
					       				的同事<span class="num">${csItem.count}人</span>
					       			</c:when>
					       			<c:otherwise>
										<a href=${applicationScope.urlBrowse}/searchEx.do?ref=sg_guide_${csItem.type}_search&amp;${csItem.moreLink} target="_blank">${csItem.typeName}</a> 
					       				的同学<span class="num">${csItem.count}人</span>	
									</c:otherwise>
								</c:choose>	
					       	</h4>
							<ul class="clearfix" id="guids-uses-con${status.index}"></ul>
					    </div>
					    <c:if test="${csItem.type!='location'}">
							<div id="showLessCon${status.index}" class="show-more">
								<a href=${applicationScope.urlBrowse}/searchEx.do?ref=sg_guide_${csItem.type}_search&amp;${csItem.moreLink} target="_blank">
									显示更多&raquo;
								</a>
							</div>
						</c:if>					
					</c:if>		
				</c:when>
			</c:choose>
		</c:forEach>	
	</c:if>	
	<c:if test="${fn:length(sameIPList)>0}">				
		<div class="users">
	       	<h4>在你附近上网</h4>
			<ul class="clearfix" id="guids-uses-con99"></ul>
	    </div>
		<div id="showLessCon99" class="show-more" style="display:none"></div>		
	</c:if>	
</div>
<c:if test="${requestScope.guide_social=='social_bad'}">
	<%@ include file="/views/guide/xf/common/middle/guide_hot_share.jsp"%>
</c:if>
                            	
