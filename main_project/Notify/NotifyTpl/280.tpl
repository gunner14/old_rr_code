{{#HEAD}}
<div id="requests_huodong_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/hi.gif">通知<span>{{280_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=280&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});
	}
	ReqCenter.regReqType('huodong:280', {
		xxx: function(info){
			var el = dom.wrap(info.el);
			var nid = info.params;
			el.dispose();
			delNotify(nid);
		}
	});
});
</script>
<ul id="requests_huodong_list" class="requests-list">
{{>LI_280}}
</ul>
{{#MORE}}
<div class="showmore" style="display:none;">
	<a data-page="1" data-count="{{280_count}}" onclick="ReqCenter.funCall('req_more', 'huodong', this)" class="requests-show-more">显示更多vip通知</a>
</div>
{{/MORE}}
