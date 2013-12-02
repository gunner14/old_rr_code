{{#HEAD}}
<div class="requests-header" id="requests_showLove_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/lovehouse.gif">感情状态请求<span>{{325_count}}</span></h2>
</div>
{{/HEAD}}
<ul id="requests_showLove_list" class="requests-list">
{{>LI_325}}
</ul>

<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=325&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
  function acceptFriend(leId, loveId, nid, cFun){
  	var dispatchFeed;
  	if ($('feed_'+ nid).checked) {
  		dispatchFeed = 1;
  	}else{
  		dispatchFeed = 0;
  	}
  	
    new XN.NET.xmlhttp({
      url:'http://www.' + XN.env.domain + '/loveinfo/accept?lifeEventId='+ leId +'&loveInfoId='+ loveId +'&dispatchFeed='+ dispatchFeed,
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
  function rejectFriend(leId, loveId, nid, cFun){
  	var dispatchFeed;
  	if ($('feed_'+ nid).checked) {
  		dispatchFeed = 1;
  	}else{
  		dispatchFeed = 0;
  	}
  	
    new XN.NET.xmlhttp({
      url:'http://www.' + XN.env.domain + '/loveinfo/deny?lifeEventId=' + leId + '&loveInfoId=' + loveId +'&dispatchFeed='+ dispatchFeed,
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
  ReqCenter.regReqType('showLove:325', {
    accept: function(info){
      var el = dom.wrap( info.el );
      var leId = info.params[0];
      var loveId = info.params[1];
      var nid = info.params[2];

      acceptFriend(leId, loveId, nid, function(){
        var btns = dom.getElement('div.btns', el);
        var detail = dom.getElement('div.section', el);
        detail.innerHTML ="已经接受了该邀请";
        btns.hide();
      });
    },
    refuse: function(info){
      var el = dom.wrap( info.el );
      var leId = info.params[0];
      var loveId = info.params[1];
      var nid = info.params[2];
      
      rejectFriend(leId, loveId, nid, function(){
        var btns = dom.getElement('div.btns', el);
        var detail = dom.getElement('div.section', el);
        detail.innerHTML="已经忽略了该邀请";
        btns.hide();
      });
    },
    xxx: function(info){
      var el = dom.wrap(info.el);
      var leId = info.params[0];
      var loveId = info.params[1];
      var nid = info.params[2];
      
      rejectFriend(leId, loveId, nid, function(){
        el.dispose();
      });
    }
  });
}); 
</script>
{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{325_count}}" onclick="ReqCenter.funCall('req_more', 'showLove', this)" class="requests-show-more">显示更多感情状态请求</a>
</div>
{{/MORE}}
