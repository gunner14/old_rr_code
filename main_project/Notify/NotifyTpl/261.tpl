{{#HEAD}}
<div id="requests_xiaozuInvite_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/group.gif">小组邀请<span>{{261_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom,net', function(exports, dom,net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=261&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
  function acceptCircle(acceptUrl, callback){
    new XN.NET.xmlhttp({
      url:acceptUrl,
      onSuccess:function(r){
        var rs = XN.json.parse(r.responseText);
        var code = rs.code;
        if(code==0){
          callback(rs);
        }else{
           XN.DO.alert({title:'提示',modal:true,message:rs.msg});
        }
      }
    });
  }     
  function refuseCircle(rejUrl, callback){
    new XN.NET.xmlhttp({
      url:rejUrl,
      onSuccess:function(r){
        var rs = XN.json.parse(r.responseText);
        var code = rs.code;
        if(code==0){
          callback();
        }else{
          XN.DO.alert({title:'提示', modal:true, message:rs.msg});
        }
      }
    });
  }   
  ReqCenter.regReqType('xiaozuInvite:261', {
    accept: function(info){
      var url = info.params[0];
      var nid = info.params[1];
      var el = dom.wrap(info.el);
      var xiaozu_url = el.getData('link');
      acceptCircle(url, function(rs){
        var btns = dom.getElement('.btns', el);
        var detail = dom.getElement('.detail', el);

        var HTML = '<h4 class="header">成功加入小组，<a href="'+ xiaozu_url +'" target="_blank">立即去看看</a></h4>';
        HTML = HTML + rs.msg
        detail.innerHTML = HTML;
        btns.hide();
        delNotify(nid);
      });
    },
    refuse: function(info){
      var url = info.params[0];
      var nid = info.params[1];
      var el = dom.wrap(info.el);
      refuseCircle(url, function(rs){
        var btns = dom.getElement('.btns', el);
        var detail = dom.getElement('.detail', el);

        detail.innerHTML = '小组请求已忽略';  
        btns.hide();
        delNotify(nid);
      });
    },
    xxx: function(info){
      var url = info.params[0];
      var nid = info.params[1];
      var el = dom.wrap(info.el);
      refuseCircle(url, function(rs){
        el.dispose();
        delNotify(nid);
      });
    }
  });
}); 
</script>

<ul id="requests_xiaozuInvite_list" class="requests-list">
{{>LI_261}}
</ul>


{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{261_count}}" onclick="ReqCenter.funCall('req_more', 'xiaozuInvite', this)" class="requests-show-more">显示更多小组邀请</a>
</div>
{{/MORE}}
