<%@ page pageEncoding="UTF-8" %>
<c:if test="${visiter.stage == 20}">
<div id="adbar">
	<c:if test="${empty cleanpage}">
		<%@ include file="/inc/promo-narrow-new.inc" %>
		<script type="text/javascript" src="${applicationScope.urlSmallStatic}/js/activity.js"></script>
		<%@ include file="/inc/promo-hotcommend.inc" %>
		<%@ include file="/inc/promo-narrow2-new.inc" %>

		<c:if test="${!empty partner}">
			<div class="block announcement link">
				<h3>合作伙伴</h3>
				<div>
					<p><a target="_blank" href="http://www.kaifulee.com/"><img src="${applicationScope.urlSmallStatic}/img/home/kaifu.gif"/></a></p>
				</div>
			</div>
		</c:if>
	</c:if>
</div>
</c:if>