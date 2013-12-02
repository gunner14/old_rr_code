{{#HEAD}}
<div id="requests_tuijian_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/friend.gif">好友推荐<span>{{260_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom,net', function(exports, dom, net){
  window.addFriendInSpecialEnvioerment = '89898';
  function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=260&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
  function rejectRecommend(r, m, cFun){
    new XN.NET.xmlhttp({
      url:"http://friend.renren.com/j_f_deny_rcd",
      data:'r='+ r +'&s='+ m,
      onSuccess:function(r){
        var rs = XN.JSON.parse(r.responseText);
        if(rs.rc==1){
          cFun(rs);
        }else{
          XN.DO.showError(rs.msg);
        }
      },
      onError:function(){
        XN.DO.showError( "忽略时出现错误，请重试或者联系管理员" );
      }
    });
  }
  ReqCenter.regReqType('tuijian:260', {
    accept: function(info){
      var id = info.params[0],
      name = info.params[1],
      src = info.params[2];
      matchmaker = info.params[3];
      nid = info.params[4];
      var el = dom.wrap(info.el);

      showRequestFriendDialog({
        id: id,
        name: name,
        head: src,
        matchmaker: matchmaker,
        callback: function(){
          var detail = dom.getElement('div.detail', el);
          var btns = dom.getElement('div.btns', el);
          btns.hide();
          detail.innerHTML="接受了推荐的好友：<a href='http://www.renren.com/profile.do?id="+ id +"'><strong> "+ name +" </strong></a>";
          delNotify(nid);
        }
      });
    },
    refuse: function(info){
      var r_id = info.params[0],
      r_name = info.params[1],
      m_id = info.params[2];
      nid = info.params[3];
      var el = dom.wrap(info.el);

      rejectRecommend(r_id, m_id, function(rs){
        var detail = dom.getElement('div.detail', el);
        var btns = dom.getElement('div.btns', el);
        btns.hide();
        detail.innerHTML="忽略了推荐的好友<a href='http://www.renren.com/profile.do?id="+ r_id+"'><strong> "+ r_name+" </strong></a>";
        delNotify(nid);
      });
    },
    xxx: function(info){
      var r_id = info.params[0],
      m_id = info.params[1];
      nid = info.params[2];
      var el = dom.wrap(info.el);

      rejectRecommend(r_id, m_id, function(rs){
        el.dispose();
        delNotify(nid);
      });
    }
  });
}); 
</script>

<ul id="requests_tuijian_list" class="requests-list">
{{>LI_260}}
</ul>


{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{260_count}}" onclick="ReqCenter.funCall('req_more', 'tuijian', this)" class="requests-show-more">显示更多推荐的好友</a>
</div>
{{/MORE}}
