{{#HEAD}}
<div id="requests_vip_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/vip.gif">VIP通知<span>{{283_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=283&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});
	}
	ReqCenter.regReqType('vip:283', {
		xxx: function(info){
			var el = dom.wrap(info.el);
			var nid = info.params;
			el.dispose();
			delNotify(nid);
		}
	});
});
</script>
<ul id="requests_vip_list" class="requests-list">
{{>LI_283}}
</ul>
{{#MORE}}
<div class="showmore" style="display:none;">
	<a data-page="1" data-count="{{283_count}}" onclick="ReqCenter.funCall('req_more', 'vip', this)" class="requests-show-more">显示更多VIP通知</a>
</div>
{{/MORE}}
