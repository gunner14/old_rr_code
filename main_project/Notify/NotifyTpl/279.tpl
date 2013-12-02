{{#HEAD}}
<div class="requests-header" id="requests_xiaozhan_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/zhan.gif">小站推荐请求<span>{{279_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=279&nid=' + nid,
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
			url:'http://zhan.renren.com/'+ url +'/recommend/accept',
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
			url:'http://zhan.renren.com/'+ url +'/recommend/reject',
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
	ReqCenter.regReqType('xiaozhan:279', {
		accept: function(info){
			var el = dom.wrap( info.el );
			var url = info.params[0];
			var id = info.params[1];
			var siteId = info.params[2];
			var nid = info.params[3];

			acceptFriend(url, id, siteId, nid, function(){
				var btns = dom.getElement('div.btns', el);
				var header = dom.getElement('h4.header', el);
				var detail = dom.getElement('div.detail', el);
				header.innerHTML ="成功关注小站，<a href='http://zhan.renren.com/"+ url +"?from=req' target='_blank'>过去看看</a>";
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
				header.innerHTML ="已经拒绝了该推荐";
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

<ul id="requests_xiaozhan_list" class="requests-list">
{{>LI_279}}
</ul>

{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{279_count}}" onclick="ReqCenter.funCall('req_more', 'xiaozhan', this)" class="requests-show-more">显示更多小站推荐请求</a>
</div>
{{/MORE}}
