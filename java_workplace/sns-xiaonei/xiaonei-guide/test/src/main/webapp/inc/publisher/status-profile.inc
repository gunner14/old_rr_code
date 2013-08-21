<%@page pageEncoding="UTF-8" %>
<%@ taglib uri="http://www.xiaonei.com/tags-core" prefix="xn" %>
<%-- 状态，个人主页使用 --%>
<div class="m-composer">	
	<div class="m-composer-header">
		<div class="statuspanel">
			<%@include file="/inc/publisher/status-common.inc" %>
		</div>
		<%--<%@ include file="/inc/publisher/publisher_infoFin.inc"%>--%>
	</div>
	<div id="publisherModule" class="m-composer-content" style="display:none;">
	</div>
	<!--module composer end-->
</div>