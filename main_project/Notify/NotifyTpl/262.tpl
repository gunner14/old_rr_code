{{#HEAD}}
<div class="requests-header" id="requests_appmessage_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/app.png">应用请求<span>{{262_count}}</span></h2>
</div>
{{/HEAD}}
<ul id="requests_appmessage_list" class="requests-list">
{{>LI_262}}
</ul>

<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=262&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
  function acceptFriend(requestId, appId, type, nid, cFun){
    new XN.NET.xmlhttp({
      url:'http://app.' + XN.env.domain + '/request/handleRequest.do?rid='+requestId+'&appId='+appId+'&type='+type,
      method:'post',
      onSuccess:function(r){
        var r = XN.JSON.parse(r.responseText);
        cFun();
        delNotify(nid);
      },
      onError:function(){
        XN.DO.showError('网络通信失败,请重试');
      }
    }); 
  }
  function rejectFriend(requestId, appId, type, nid, cFun){
    new XN.NET.xmlhttp({
      url:'http://app.' + XN.env.domain + '/request/ignoreAppRequest.do?rid=' + requestId + '&appId=' + appId + '&type= ' +type,
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
  ReqCenter.regReqType('appmessage:262', {
    accept: function(info){
      var el = dom.wrap( info.el );
      var requestId = info.params[0];
      var appId = info.params[1];
      var type = info.params[2];
      var url = info.params[3];
      var nid = info.params[4];

      acceptFriend(requestId, appId, type, nid, function(msg){
        var btns = dom.getElement('div.btns', el);
        var detail = dom.getElement('div.detail', el);
        detail.innerHTML ="已经接受了该邀请";
        btns.hide();
        window.location.href = url;
      });
    },
    refuse: function(info){
      var el = dom.wrap( info.el );
      var requestId = info.params[0];
      var appId = info.params[1];
      var type = info.params[2];
      var nid = info.params[3];
      
      rejectFriend(requestId, appId, type, nid, function(){
        var btns = dom.getElement('div.btns', el);
        var detail = dom.getElement('div.detail', el);
        detail.innerHTML="已经忽略了该邀请";
        btns.hide();
      });
    },
    xxx: function(info){
      var el = dom.wrap(info.el);
      var requestId = info.params[0];
      var appId = info.params[1];
      var type = info.params[2];
      var nid = info.params[3];
      
      rejectFriend(requestId, appId, type, nid, function(){
        el.dispose();
      });
    }
  });
}); 
</script>
{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{262_count}}" onclick="ReqCenter.funCall('req_more', 'appmessage', this)" class="requests-show-more">显示更多应用请求</a>
</div>
{{/MORE}}
