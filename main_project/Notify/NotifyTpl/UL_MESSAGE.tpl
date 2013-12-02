{{#HEAD}}
<div id="requests_leave_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/leave.gif">留言回复提醒<span>{{REMIND_NUM}}</span></h2><a href="javascript:;" id="delMessage"></a>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){

	XN.dom.ready(function(){
		getList();
		delContent();
		readContent();
	});
	
	function getList(){
		new XN.net.xmlhttp({
			url: 'http://notify.renren.com/rmessage/get?getbybigtype=1&bigtype=1&limit=21&begin=0&view=16&rand='+Math.random(),
			method: 'get',
			onSuccess:function(r){
				var data = XN.JSON.parse(r.responseText),
					array = [],
					n = 0,
					len,
					ul = $('requests_leave_list');
				
				if (data.length > 20){
					$('leave_showmore').show(); //取11条，是为了看有没有更多
					len = data.length - 1;
				}else{
					len = data.length;
				}
				
				
				for (var i = 0; i < len; i++) {
					var msg = data[i],
						t = changeTime(msg.time);

					array[n++] = ['<li class="title">'+ t +'</li>'+ getMsgHtml(msg)].join('');
				}
				ul.innerHTML = array.join('');
				delRepeatTitle();		
			},
			onError:function(){
				XN.DO.showError('网络通讯失败，请稍后再试');
			}
		});
	}
	
	function changeTime(t){
		var d = new Date(t*1000) || new Date();
		var time = d.getFullYear() +'年'+ (d.getMonth()+1) +'月'+ d.getDate() +'日';
		return time;
	}
	
	// 获取某条请求的html
	function getMsgHtml(msg) {
		var msgHtml = '',
			unreadcount = '';
		
		//显示合并
		if (msg.unreadcount > 1) {
			unreadcount = ['<span style="color:#9C9A9C;margin-left:9px;font-weight:blod;">' + msg.unreadcount + '条</span>'].join('');
		}else{
			unreadcount = '';
		}
		
		msgHtml = [	'<li class="'+ (msg.unread == '1' ? "unread" : "request" ) +'"><h4 class="header" id="notify_' + msg.nid + '"><img src="' + msg.icon + '"> ' + msg.content +''+ unreadcount +'</h4>'
				+	'<div class="btn-x">'
				+ 	'	<a id="remove_' + msg.nid + '" href="javascript:;" data-api="' + msg.rmessagecallback + '" data-read="'+ msg.processcallback +'" class="x-to-delete"></a>'
				+ 	'</div></li>'].join();
				
		return msgHtml;
	}
	
	// 时间相同的话，删除标题
	function delRepeatTitle() {
		var p = Sizzle('.title');
		
		for (var i = 0; i < p.length; i++) {
			for (var j = p.length - 1; j > i; j--) {
				if (p[i].innerHTML == p[j].innerHTML) {
					dom.wrap(p[j]);
					p[j].dispose();
				}
			}
		}
	}
	
	//删除这条信息
	function delContent(){
		var con = dom.getElement('#requests_leave_list');
		con.delegate('li', 'click', function(e) {
			var el = dom.wrap(e.target);
			if(!Sizzle('.x-to-delete',el.getParent('li'))[0]) return;
			
			var url = Sizzle('.x-to-delete',el.getParent('li'))[0].getAttribute("data-api");
			
			if (el.nodeName.toLowerCase() == "a" && el.getAttribute("data-api")) {
				if (el.getParent('li').className == 'unread') {
					XN.DO.confirm({
						title: '提示',
						message: '你还没有处理过此条留言提醒，删除后，你将无法找回此条留言提醒，是否确认删除？',
						callback: function(yes){
							if(yes){
								new XN.net.xmlhttp({
									url: url,
									method: 'post',
									onSuccess:function(r){
										var data = XN.JSON.parse(r.responseText);
							
										if (data == '0'){
											el.getParent('li').dispose();
										}
									},
									onError:function(){
										XN.DO.showError('网络通讯失败，请稍后再试');
									}
								});
							}
						}
					});
				}else{
					new XN.net.xmlhttp({
						url: url,
						method: 'post',
						onSuccess:function(r){
							var data = XN.JSON.parse(r.responseText);
							
							if (data == '0'){
								el.getParent('li').dispose();
							}
						},
						onError:function(){
							XN.DO.showError('网络通讯失败，请稍后再试');
						}
					});
				}
			}
			
			
		});
	}
	
	//设为已读
	function readContent() {
		var con = dom.getElement('#requests_leave_list');
		con.delegate('li', 'click', function(e) {
			var el = dom.wrap(e.target);
			if(!Sizzle('.x-to-delete',el.getParent('li'))[0]) return;
			var readUrl = Sizzle('.x-to-delete',el.getParent('li'))[0].getAttribute("data-read");
			
			if (el.getParent('li').className == 'unread') {
				if (el.nodeName.toLowerCase() == "a" && el.getAttribute("source")) {
					new XN.net.xmlhttp({
						url: readUrl,
						method: 'post',
						onSuccess:function(r){
							var data = XN.JSON.parse(r.responseText);
							
							if (data == '0'){
								el.getParent('li').className = 'request';
							}
						},
						onError:function(){
							XN.DO.showError('网络通讯失败，请稍后再试');
						}
					});
				}
			}

		});
	}

	ReqCenter.regReqType('leave:251', {});
});
</script>
<ul id="requests_leave_list" class="requests-list">
</ul>
{{#MORE}}
<div class="showmore" id="leave_showmore" style="display:none;">
	<a data-page="1" data-count="{{REMIND_NUM}}" onclick="ReqCenter.funCall('req_more', 'leave', this)" class="requests-show-more">显示更多留言回复提醒</a>
</div>
{{/MORE}}
