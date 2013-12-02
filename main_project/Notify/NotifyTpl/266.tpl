{{#HEAD}}
<div class="requests-header" id="requests_page_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/page.gif">主页管理员邀请<span>{{266_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=266&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
	function acceptFriend(id, cFun){
		new XN.NET.xmlhttp({
			url:'http://page.renren.com/admininvite/accept?uuid='+ id,
			method:'post',
			onSuccess:function(r){
				var r = XN.JSON.parse(r.responseText);
				cFun(r.msg);
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
	// 拒绝好友请求
	function rejectFriend(id,cFun){
		new XN.NET.xmlhttp({
			url:'http://page.renren.com/admininvite/ignore?uuid='+id,
			method:'post',
			onSuccess:function(r){
				cFun();
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});			
	}
	ReqCenter.regReqType('page:266', {
		accept: function(info){
			var el = dom.wrap( info.el );
			var id = info.params[0];
			var pageName = info.params[1];
			var pageId = info.params[2];
			var nid = info.params[3];

			acceptFriend(id, function(msg){
				var btns = dom.getElement('div.btns', el);
				var detail = dom.getElement('div.section', el);
				if(msg == "成功")
				{
					detail.innerHTML ="已经接受邀请，成为 <a href='http://page.renren.com/"+ pageId +"' target='_blank'>"+ pageName +"</a> 的管理员";
				}else{
					detail.innerHTML = msg;
				}
				btns.hide();
				delNotify(nid);
			});
		},
		refuse: function(info){
			var el = dom.wrap( info.el );
			var id = info.params[0];
			var nid = info.params[1];
			
			rejectFriend(id, function(){
				var btns = dom.getElement('div.btns', el);
				var detail = dom.getElement('div.detail', el);
				detail.innerHTML="已经拒绝了该邀请";
				btns.hide();
				delNotify(nid);
			});
		},
		xxx: function(info){
			var el = dom.wrap(info.el);
			var id = info.params[0];
			var nid = info.params[1];
			rejectFriend(id, function(){
				el.dispose();
				delNotify(nid);
			});
		}
	});
});	
</script>

<ul id="requests_page_list" class="requests-list">
{{>LI_266}}
</ul>


{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{266_count}}" onclick="ReqCenter.funCall('req_more', 'page', this)" class="requests-show-more">显示更多公共主页管理邀请</a>
</div>
{{/MORE}}
