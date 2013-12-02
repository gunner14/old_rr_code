{{#HEAD}}
<div class="requests-header" id="requests_apppage_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/app.png">关联请求<span>{{373_count}}</span></h2>
</div>
{{/HEAD}}
<ul id="requests_apppage_list" class="requests-list">
{{>LI_373}}
</ul>

<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=373&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
	
	function acceptFriend(url, appId, pageId, signature, receiverId, notify_id, nid, cFun){
		new XN.NET.xmlhttp({
			url:'http://'+ url,
			data:'app_id='+ appId +'&page_id='+ pageId +'&signature='+ signature +'&receiverId='+ receiverId +'&notify_id='+ notify_id,
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

	function rejectFriend(url, appId, pageId, signature, receiverId, notify_id, nid, cFun){
		new XN.NET.xmlhttp({
			url:'http://'+ url,
			data:'app_id='+ appId +'&page_id='+ pageId +'&signature='+ signature +'&receiverId='+ receiverId +'&notify_id='+ notify_id,
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
	
  ReqCenter.regReqType('apppage:373', {
  	accept: function(info){
		var el = dom.wrap( info.el );
			url = info.params[0],
			appId = info.params[1],
			pageId = info.params[2],
			signature = info.params[3],
			receiverId = info.params[4],
			notify_id = info.params[5],
			settingUrl = info.params[6],
			nid = info.params[7];
		
		acceptFriend(url, appId, pageId, signature, receiverId, notify_id, nid, function(){
			var btns = dom.getElement('div.btns', el);
			var detail = dom.getElement('div.detail', el);
			detail.innerHTML ="已接受关联请求，快去 <a href="+ settingUrl +" target='_blank'>添加模块>></a>";
			btns.hide();
		});
	},
	refuse: function(info){
		var el = dom.wrap( info.el ),
			url = info.params[0],
			appId = info.params[1],
			pageId = info.params[2],
			signature = info.params[3],
			receiverId = info.params[4],
			notify_id = info.params[5],
			nid = info.params[6];
		
		rejectFriend(url, appId, pageId, signature, receiverId, notify_id, nid, function(){
			var btns = dom.getElement('div.btns', el);
			var detail = dom.getElement('div.detail', el);
			detail.innerHTML="已拒绝关联请求";
			btns.hide();
		});
	},
	xxx: function(info){
		var el = dom.wrap(info.el);
			url = info.params[0],
			appId = info.params[1],
			pageId = info.params[2],
			signature = info.params[3],
			receiverId = info.params[4],
			notify_id = info.params[5],
			nid = info.params[6];
		
		rejectFriend(url, appId, pageId, signature, receiverId, notify_id, nid, function(){
			el.dispose();
		});
	}

  });
}); 
</script>
{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{373_count}}" onclick="ReqCenter.funCall('req_more', 'apppage', this)" class="requests-show-more">显示更多关联请求</a>
</div>
{{/MORE}}
