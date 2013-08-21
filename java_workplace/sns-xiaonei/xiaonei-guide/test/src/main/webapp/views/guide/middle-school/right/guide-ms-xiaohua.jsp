<%@ page pageEncoding="utf-8" %>
<c:if test="${!empty requestScope.cityName && requestScope.cityName!=''}">
	<div class = "side-item Cityxiaohua" id = "Cityxiaohua">
		<div class = "city-header ">
			<c:choose>
				<c:when test="${fn:length(requestScope.cityName)<=7}">					
					<h4>${requestScope.cityName}的校花校草</h4>
				</c:when>
				<c:otherwise>
					<h4>${fn:substring(requestScope.cityName, 0, 6)}...的校花校草</h4>
				</c:otherwise>
			</c:choose>		
			<a href ="http://mm.renren.com?from=guide" target="_blank">进入应用</a>
		</div>
		<div class = "city-tab" > 
			<ul class ="filter">
				<li class = "s"><a href="#nogo" id ="flower" ><span>校花</span></a></li>
				<li><a href="#nogo" id ="grass"><span>校草</span></a></li>
			</ul>	
		</div>
		<div class ="photo-list" >
			<ul  id = "face-list">            
	  		</ul>
	  		<p id="no-grass" style="color:#888888;margin-top:8px;display:none" >抱歉，这个城市还没有合格的校草~O(∩_∩)O~ </p>
			<p id="no-flower" style="color:#888888;margin-top:8px;display:none" >抱歉，这个城市还没有合格的校花~O(∩_∩)O~ </p>	
		</div>			
	</div>
</c:if>
