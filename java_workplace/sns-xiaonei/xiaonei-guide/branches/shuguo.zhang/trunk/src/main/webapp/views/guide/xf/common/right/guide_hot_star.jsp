<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<c:if test="${domainIdentified['sys.domain']=='kaixin.com'}">
	<div class="home-sidebar" id="kaixinStarUserRecom_con">
		<div class="side-item recomApp">
			<div class="side-item-header clearfix">
				<h4>人气之星</h4>
				<div class="option">
					<a href="javascript:;" id="kaixinStarUserRecom_btn">刷新</a>
				</div>
			</div>
			<div class="side-item-body clearfix">
				<ul id="kaixinStarUserRecom">
					<%@ include file="/views/guide/xf/common/right/guide_star_li.jsp"%>		
				</ul>
			</div>
		</div>
	</div>
</c:if>