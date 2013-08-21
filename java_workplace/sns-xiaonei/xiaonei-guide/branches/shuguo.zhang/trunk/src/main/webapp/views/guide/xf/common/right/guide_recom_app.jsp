<%@ page pageEncoding="utf-8" %>
<c:if test="${domainIdentified['sys.domain']=='kaixin.com'}">
	<div class="side-item recomApp">
		<div class="side-item-header clearfix">
			<h4>推荐游戏</h4>
			<div class="option"><a href="javascript:void(0);" id="kaixinHotApp_btn">刷新</a></div>
		</div>
		<div class="side-item-body clearfix">
			<ul id="kaixinHotApp"></ul>
		</div>
	</div>	
</c:if>
