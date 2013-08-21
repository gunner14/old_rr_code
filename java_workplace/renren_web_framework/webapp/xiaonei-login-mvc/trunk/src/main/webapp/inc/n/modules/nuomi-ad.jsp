<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<c:set var="currentTime"><%=System.currentTimeMillis() %></c:set>
<c:set var="timeShowCola" value="1295452800698" />
<c:set var="timeHideCola" value="1295711999800" />
<c:set var="showColaBanner" value="${currentTime >= timeShowCola && currentTime <= timeHideCola}" />
<c:choose>
	<c:when test="${showColaBanner}">
		<a style="margin-top:10px;display:block;width:145px;height:179px;" href="http://track.yx.renren.com/statmop/dfa1?dfaid=7561" target="_blank">
			<img src="http://a.xnimg.cn/imgpro/home/cola_banner.jpg" width="145" height="179" alt="可口可乐 - 新愿欢享中国年" title="可口可乐 - 新愿欢享中国年" />
		</a>
	</c:when>
	<c:otherwise>
		<div id="ad100000000067">
		</div>
	</c:otherwise>
</c:choose>