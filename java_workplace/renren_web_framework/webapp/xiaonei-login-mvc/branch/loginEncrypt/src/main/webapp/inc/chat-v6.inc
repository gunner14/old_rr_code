<c:if test="${!empty requestScope.host}">
	<c:if test="${INC_NO_WEBPAGER != true}">
	<div id="console_log" style="position:absolute;top:0;left:0;"></div>

		<%--部分错误卸载IE7或8的IE6用户不加载webpager--%>
		<!--[if gt IE 6]><script>if(window.ActiveXObject&&!window.XMLHttpRequest)XN.disableWebpager=true;</script><![endif]-->

		<%-- 节点用户不显示webpager --%>
		<c:set var="visitorIdString" value="${visiter.id}, " />
		<c:set var="coreUserIds1" value="287285057, 287286115, 287286312, 287510926, 287284111, 287286920, 287285924, 287286782, 287284388, 287286452, 287394429, 287516742, 287519876, 287285254, 287508269, " />
		<c:set var="coreUserIds2" value="249181025, 220882929, 223754734, 244070408, 202100889, 272836854, 232586810, 223043377, 239730520, 258773436, 268124707, 271872081, " />
		<c:set var="coreUserIds3" value="265159050, 271274636, 221264833, 256304620, 242867520, 314009558, 262553255, 238112863, 241815186, 226074748, 309678036, 236066776, " />
		<c:set var="coreUserIds4" value="227758693, 244520228, 223050986, 256097974, 230845847, 224245930, 225859971, 231653355, 240233492, 320418683, 232318765, 227757798, " />
		<c:set var="coreUserIds5" value="253613919, 253639138, 260818477, 240986966, 263700941, 262016809, 278943112, 234902010, 282646239, 241815186, 241034932, 286801539, " />
		<c:set var="coreUserIds" value="${coreUserIds1}${coreUserIds2}${coreUserIds3}${coreUserIds4}${coreUserIds5}" />
		<c:if test="${fn:contains(coreUserIds, visitorIdString)}">
			<script type="text/javascript">XN.disableWebpager=true</script>
		</c:if>

	<iframe id="imengine" name="imengine" src="http://wpi.renren.com/wtalk/ime.htm?v=5" frameBorder="0" style="position:absolute;left:-1000pt;top:20pt;width:200pt;height:100pt;"></iframe>

	<div id="bottombar"></div>
	</c:if>
</c:if>
