<%@page pageEncoding="UTF-8" %>
<div class="side-item pymk pymk-home" style="display:none" id="pymk_home">
	<div class="side-item-header pymk-header clearfix">
		<h4>推荐</h4>
		<div class="option">
			<a href="#" class="pager-back pager-back-disable" onclick="return false;" title="上一页"></a>
			<a href="#" class="pager-forward pager-forward-disable" onclick="return false;" title="下一页"></a>
			<a href="${applicationScope.urlFriend}/myfriendlistx.do?portal=4#item_1" class="more" stats="home_friend_recommend_new" title="更多"></a>
		</div>
	</div>
	<div class="side-item-body pymk-body clearfix">
		<ul class="recognize" id="commonFriends_home"></ul>
	</div>
</div>
<script type="text/javascript">
	object.use('xn.pymk, dom', function(xn, dom) {
		dom.ready(function() {	
			XN.namespace('app');
			var params = {};
			var frds = parseInt('${friendCount}');
			//if (frds > 200 || window.screen.availHeight <= 768) {
				// v6用户屏幕分辨率小于等于768时第一个推荐位展示两行
				params.amount = 3;
		//	}
			XN.app.pymk = new xn.pymk.PYMK(params);
			// 兼容
			XN.app.pymk.remove = XN.app.pymk.remove.bind(XN.app.pymk);
			XN.app.pymk.removeFromAdd = XN.app.pymk.removeFromAdd.bind(XN.app.pymk);
			XN.app.pymk.gossip = XN.app.pymk.gossip.bind(XN.app.pymk);
			window.requestPageFriend = XN.app.pymk.requestPageFriend.bind(XN.app.pymk);
			window.logRcd = xn.pymk.logRcd;
			window.refreshCodeInRequestPageFriend = xn.pymk.refreshCodeInRequestPageFriend;
		});
	});
</script>
