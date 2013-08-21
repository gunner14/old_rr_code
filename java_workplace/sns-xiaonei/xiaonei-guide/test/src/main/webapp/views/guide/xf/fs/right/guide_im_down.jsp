<%@ page pageEncoding="utf-8" %>
<c:if test="${domainIdentified['siteNameAbbr']=='rr'}">
	<div class="side-item requests">
		<div class="side-item-header clearfix">
			<h4>人人桌面</h4>
			<div class="option">
				<a href="http://im.${applicationScope.domain}/desktop27.html" target="_black">详情</a>
			</div>
		</div>		
		<div class="side-item-body clearfix">
			<p>改状态、定时偷菜，人人网官方聊天、游戏工具，5秒即可完成下载。<br/>
				<a href="http://im.${applicationScope.domain}/desktop/rrsetup-27.exe" class="imDown" onclick="ajax_stat_nur_link('gp_o_downim')" target="_black"></a>
			</p>
		</div>
	</div>
</c:if>
