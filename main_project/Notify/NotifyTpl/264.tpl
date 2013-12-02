{{#HEAD}}
<div id="requests_pageInvite_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/page.gif">主页推荐<span>{{264_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=264&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
	function acceptFriend(id, pid, cFun){
		new XN.NET.xmlhttp({
			url:'http://page.renren.com/apply/accept?rid='+ id +'&pid='+ pid,
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
	// 拒绝公共主页邀请
	function rejectFriend(id, pid, cFun){
		new XN.NET.xmlhttp({
			url:'http://page.renren.com/apply/ignore?rid='+ id +'&pid='+ pid,
			method:'post',
			onSuccess:function(r){
				cFun();
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});
	}
	ReqCenter.regReqType('pageInvite:264', {
		accept: function(info){
			var el = dom.wrap( info.el );
			var id = info.params[0];
			var pageName = info.params[1];
			var pageId = info.params[2];
			var nid = info.params[3];

			acceptFriend(id, pageId, function(msg){
				var btns = dom.getElement('div.btns', el);
				var detail = dom.getElement('div.section', el);
				if(msg == "关注发送成功")
				{
					detail.innerHTML ="已经关注了公共主页<a href='http://page.renren.com/"+ pageId +"' target='_blank'>"+ pageName +"</a>，<a href='http://page.renren.com/"+ pageId +"' target='_blank'>过去看看</a>";
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
			var pageId = info.params[1];
			var nid = info.params[2];
			
			rejectFriend(id, pageId, function(){
				var btns = dom.getElement('div.btns', el);
				var detail = dom.getElement('div.section', el);
				detail.innerHTML="已经拒绝了该邀请";
				btns.hide();
				delNotify(nid);
			});
		},
		xxx: function(info){
			var el = dom.wrap(info.el);
			var id = info.params[0];
			var pageId = info.params[1];
			var nid = info.params[2];
			
			rejectFriend(id, pageId, function(){
				el.dispose();
				delNotify(nid);
			});
		}
	});
});	
</script>

<ul id="requests_pageInvite_list" class="requests-list">
{{>LI_264}}
</ul>

{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{264_count}}" onclick="ReqCenter.funCall('req_more', 'pageInvite', this)" class="requests-show-more">显示更多公共主页推荐</a>
</div>
{{/MORE}}
