<%@ page pageEncoding="utf-8" %>
<c:if test="${!empty requestScope.userStarList }">
	<div class="side-itemnew high-school-star" id="highSchoolStar">
		<div class="side-item-header-change clearfix">
	    	<h4>${requestScope.middleSchoolName}人气之星</h4>
	    	<h6>去他们主页踩一踩！</h6>
	  	</div>
	  	<div class="main-item-body">
	    	<div class="star-list-holder">
	      		<ul class="star-list clearfix">
	      			<c:forEach var="item" items="${requestScope.userStarList}" varStatus="status">
	      				<li>
	      					<a class="pop-module" href="http://www.renren.com/profile.do?id=${item.id}&from=star" k='{type :2}' target="_blank"> 
	      						<img src="${item.tiny_Url}" height="50" width="50" /> 
	      					</a> 
	      					<span class="name-holder clearfix "> 
	      						<a class="name" href="http://www.renren.com/profile.do?id=${item.id}&from=star" target="_blank" title="${item.name}">${item.name}</a>
	      					</span> 
	      					<input type="hidden" id ="ajaxid" value="${item.id}"/>
	      				</li>
	      			</c:forEach> 
	      		</ul>
	    	</div>
	  	</div>
	</div>
</c:if>
