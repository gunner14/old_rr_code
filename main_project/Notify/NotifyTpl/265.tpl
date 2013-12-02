{{#HEAD}}
<div id="requests_xiaozuApply_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/group.gif">小组申请<span>{{265_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=265&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});
	}
	ReqCenter.regReqType('xiaozuApply:265', {
		xxx: function(info){
			var el = dom.wrap(info.el);
			var nid = info.params;
			el.dispose();
			delNotify(nid);
		}
	});
});
</script>

<ul id="requests_xiaozuApply_list" class="requests-list">
{{>LI_265}}
</ul>


{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{265_count}}" onclick="ReqCenter.funCall('req_more', 'xiaozuApply', this)" class="requests-show-more">显示更多小组申请</a>
</div>
{{/MORE}}
