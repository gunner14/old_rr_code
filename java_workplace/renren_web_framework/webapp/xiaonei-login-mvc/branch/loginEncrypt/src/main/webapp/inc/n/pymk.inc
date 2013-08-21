<%@page pageEncoding="UTF-8" %>
<c:if test="${applicationScope.domain=='renren.com'}">

		<div class="side-item pymk ${otherClass}" style="display:none" id="pymk">
			<div class="side-item-header pymk-header clearfix">
				<h4>推荐</h4>
				<div class="option">
					<a href="javascript:;" class="pager-back pager-back-disable" onclick="XN.app.pymk.pagerBack();" title="上一页"></a>
					<a href="javascript:;" class="pager-forward pager-forward-disable" onclick="XN.app.pymk.pagerForward();" title="下一页"></a>
					<a href="${applicationScope.urlFriend}/myfriendlistx.do?portal=4#item_1" class="more"></a>
				</div>
			</div>
			<div class="side-item-body pymk-body clearfix">
				<ul class="recognize" id="commonFriends">
				</ul>
			</div>
		</div>

</c:if>
