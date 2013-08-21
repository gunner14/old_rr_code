<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@page import="java.util.List"%>
<%@page import="com.renren.sns.tag.view.render.SuggestSiteRender"%>

<%! 
List<SuggestSiteRender> subSuggestSiteList=null;
%>

<%
subSuggestSiteList=(List<SuggestSiteRender>)request.getAttribute("subsuggestsitelist");
%>

<div style="text-align:right;margin: 15px 90px -35px auto;" class="clearfix">
					<a href="#nogo" id="followall" style="display:block;float:right;margin-left:20px">全部关注</a>
					<span style="color:#888;display:block;float:right">共<%=subSuggestSiteList.size()%>个</span>
</div>
<div id="" class="guide-content" style="border-top:none;">
	<div id="friendPool" class="hisfriends-box" style="width:850px">
		<div class="smallsite-home" style="padding-bottom:0px;width:815px;position:relative;margin: 0 auto;">
			<div class="recommend" id="recommendSite" style="display:block;">
				<div id="tagContainer" class="recommend-content clearfix" style="padding-bottom:70px;">
				<!--小站类表 -->
					<ul id="site-list">
						
						<c:forEach var="suggest" items="<%=subSuggestSiteList%>">

							<li data-id="${suggest.id}" data-name="${suggest.profileUrl}">            
								 <a class="recommend-pic">
								 <img src="${suggest.headSource}" title="${suggest.name}" alt="${suggest.name}">
									</a>
									<h3><a title="${suggest.name}" href="#nogo">${suggest.name}</a></h3>
									<div class="recommend-descript">
									${suggest.description}
									</div>
									<p class="attention-num">${suggest.followerCount}人关注</p>
									<c:choose>
										<c:when test="${suggest.following}">
											<div class="recommend-btn2">已关注</div>
										</c:when>
								        <c:otherwise>
                                            <div class="recommend-btn"><a  href="javascript:;" class="recommend-att-btn">关注</a></div>
										</c:otherwise>
									</c:choose>
								   
								   
							</li>
						</c:forEach>							
					</ul>
				</div>
			</div>
		</div>
	</div>
</div>


