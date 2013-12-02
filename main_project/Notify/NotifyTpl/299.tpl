{{#HEAD}}
<div class="requests-header" id="requests_xzInvite_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/zhan.gif">小站创建请求<span>{{299_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=299&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
	function acceptFriend(url,id,siteId,nid,cFun){
		new XN.NET.xmlhttp({
			url:'http://zhan.renren.com/'+ url +'/zhanCreateInvite/accept',
			data:'inviterId='+ id +'&siteId='+ siteId,
			method:'post',
			onSuccess:function(r){
				cFun();
				delNotify(nid);
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});		
	}
	function rejectFriend(url,id,siteId,nid,cFun){
		new XN.NET.xmlhttp({
			url:'http://zhan.renren.com/'+ url +'/zhanCreateInvite/reject',
			data:'inviterId='+ id +'&siteId='+ siteId,
			method:'post',
			onSuccess:function(r){
				cFun();
				delNotify(nid);
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});			
	}
	ReqCenter.regReqType('xzInvite:299', {
		accept: function(info){
			var el = dom.wrap( info.el );
			var url = info.params[0];
			var id = info.params[1];
			var siteId = info.params[2];
			var siteName = info.params[3];
			var nid = info.params[4];

			acceptFriend(url, id, siteId, nid, function(){
				var btns = dom.getElement('div.btns', el);
				var header = dom.getElement('h4.header', el);
				var detail = dom.getElement('div.detail', el);
				header.innerHTML ="成功关注了人人小站<a href='http://zhan.renren.com/zhan/"+ siteId +"?from=req' target='_blank'>"+ siteName +"</a>，<a href='http://zhan.renren.com/zhan/"+ siteId +"?from=req' target='_blank'>过去看看</a>";
				detail.hide();
				btns.hide();
			});
		},
		refuse: function(info){
			var el = dom.wrap(info.el);
			var url = info.params[0];
			var id = info.params[1];
			var siteId = info.params[2];
			var nid = info.params[3];
			
			rejectFriend(url, id, siteId, nid, function(){
				var btns = dom.getElement('div.btns', el);
				var header = dom.getElement('h4.header', el);
				var detail = dom.getElement('div.detail', el);
				header.innerHTML ="已经拒绝了该邀请";
				detail.hide();
				btns.hide();
			});
		},
		xxx: function(info){
			var el = dom.wrap(info.el);
			var url = info.params[0];
			var id = info.params[1];
			var siteId = info.params[2];
			var nid = info.params[3];
			
			rejectFriend(url, id, siteId, nid, function(){
				el.dispose();
			});
		}
	});
});	
</script>

<ul id="requests_xzInvite_list" class="requests-list">
{{>LI_299}}
</ul>

{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{299_count}}" onclick="ReqCenter.funCall('req_more', 'xzInvite', this)" class="requests-show-more">显示更多小站关注请求</a>
</div>
{{/MORE}}
