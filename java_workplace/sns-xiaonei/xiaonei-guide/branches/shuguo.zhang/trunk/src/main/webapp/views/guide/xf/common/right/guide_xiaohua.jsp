<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:if test="${visiter.stage == 10}">
	<div id="xiaoTaoHua" style="display:none">
		<a href="javascript:;" class="xt-pager" onclick="nextXiaoTao();"></a>
		<div id="xiaoTaoHuaContent"></div>
	</div>
</c:if>