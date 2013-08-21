<%@page pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:if test="${ !(empty managePageList||managePageList.size()==0) }">
	<div class="side-item pymk manage">
		<div class="side-item-header clearfix">
			<h4>我管理的公共主页</h4>
			<div class="option">
				<a href="http://page.renren.com/home/ownpages">更多</a>
			</div>
		</div>
		<div class="side-item-body pymk-body clearfix">
			<ul class="recognize">
			<c:forEach var="item" items="${managePageList}">
				<li>
					<span class="headpichold">
						<a href="${applicationScope.urlPage}/${item.id}"><img width="50" height="50" src="${item.tinyUrl}"></a>
					</span>
					<span>
						<a class="name" href="${applicationScope.urlPage}/${item.id}">${item.name}</a>						
					</span>
				</li>
			</c:forEach>
			</ul>
		</div>
	</div>
</c:if>
<c:if test="${ !(empty manageLoverPageList||manageLoverPageList.size()==0) }">
	<div class="side-item pymk manage">
		<div class="side-item-header clearfix">
			<h4>我管理的情侣空间</h4>
		</div>
		<div class="side-item-body pymk-body clearfix">
			<ul class="recognize">
			<c:forEach var="item" items="${manageLoverPageList}">
				<li>
					<span class="headpichold">
						<a href="${applicationScope.urlPage}/${item.id}?Ref=home"><img width="50" height="50" src="${item.tinyUrl}"></a>
					</span>
					<span>
						<a class="name" href="${applicationScope.urlPage}/${item.id}?Ref=home">${item.name}</a>
						<p class="time">${fn:substring(item.description,0,23)}<c:if test="${fn:length(item.description)>23}">...</c:if></p>
					</span>
				</li>
			</c:forEach>
			</ul>
		</div>
	</div>
</c:if>
