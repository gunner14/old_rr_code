{{#HEAD}}
<div id="requests_notice_header" class="requests-header">
  <h2><img src="http://a.xnimg.cn/imgpro/v6/icon/hi.gif">通知<span>{{NOTICE_NUM}}</span></h2><a href="javascript:;" id="delAll"></a>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){

	XN.dom.ready(function(){
		getnoticeList();
		delnoticeContent();
	});
	
	function getnoticeList(){
		new XN.net.xmlhttp({
			url: 'http://notify.renren.com/rmessage/get?getbybigtype=1&bigtype=3&limit=16&begin=0&view=16&rand='+Math.random(),
			method: 'get',
			onSuccess:function(r){
				var r = XN.JSON.parse(r.responseText);
				var len;
				if (r.length > 10){
					$('notice_showmore').show(); //取11条，是为了看有没有更多
					len = 10;
				}else{
					len = r.length;
				}
				for (var i = 0; i < len; i++) {
					var ul = $('requests_notice_list');
					var li = document.createElement('li');
					var unreadcount = '';
					li.className = 'request';
					if (r[i].unreadcount > 1) {
						unreadcount = ['<span style="color:#9C9A9C;margin-left:9px;font-weight:blod;">' + r[i].unreadcount + '条</span>'].join('');
					}else{
						unreadcount = '';
					}
					var HTML =	[	'<h4 class="header" id="notify_' + r[i].nid + '"><img src="' + r[i].icon + '"> ' + r[i].content +''+ unreadcount +'</h4>'
											+	'<div class="btn-x">'
											+ '		<a id="remove_' + r[i].nid + '" href="javascript:;" data-api="' + r[i].rmessagecallback + '"  class="x-to-delete"></a>'
											+	'	</div>'
											+ '</div>'].join();
					li.innerHTML = HTML;
					ul.appendChild(li);
				}
			},
			onError:function(){
				XN.DO.showError('网络通讯失败，请稍后再试');
			}
		});
	}
	
	function delnoticeContent(){
		var con = dom.getElement('#requests_notice_list');
		con.delegate('.request', 'click', function(e) {
			var el = dom.wrap(e.target);
			var url = Sizzle('.x-to-delete',el.getParent('li.request'))[0].getAttribute("data-api");
			if ((el.nodeName.toLowerCase() == "a" && el.getAttribute("source")) || (el.nodeName.toLowerCase() == "a" && el.getAttribute("data-api"))) {
				new XN.net.xmlhttp({
					url: url,
					method: 'post',
					onSuccess:function(r){
						el.getParent('li.request').dispose();
					},
					onError:function(){
						XN.DO.showError('网络通讯失败，请稍后再试');
					}
				});
			}
		});
	}

	ReqCenter.regReqType('notice:280', {});
});
</script>
<ul id="requests_notice_list" class="requests-list">
</ul>
{{#MORE}}
<div class="showmore" id="notice_showmore" style="display:none;">
	<a data-page="1" data-count="{{NOTICE_NUM}}" onclick="ReqCenter.funCall('req_more', 'notice', this)" class="requests-show-more">显示更多通知</a>
</div>
{{/MORE}}
