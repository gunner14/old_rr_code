{{#HEAD}}
<div id="requests_addr_header" class="requests-header">
  <h2><img src="http://a.xnimg.cn/imgpro/v6/icon/addressbook.gif">通讯录请求<span>{{259_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
  //通讯录请求交互逻辑代码
  object.use('dom,net', function(exports, dom,net){
    function delNotify(nid){
			new XN.NET.xmlhttp({
				url:'http://notify.renren.com/rmessage/remove?type=259&nid=' + nid,
				method:'post',
				onSuccess:function(r){
				},
				onError:function(){
					XN.DO.showError('网络通信失败,请重试');
				}
			});	
		}
    function ignoreThese(ids, cFun){   //之所以放在这里实现,是这里需要调用,如果放在条目里面实现 这里调用会看起来很奇怪
      if(!cFun) return;
      var ids = [].concat(ids || []);  //接受单个的id参数
      new XN.net.xmlhttp({
        url: 'http://www.renren.com/address/ignorecard',
        method: 'post',
        //data: 'friendIds=' + ids.join('&friendIds='),
        data: 'id=' + ids,  //新接口只接受一个id
        onSuccess: function(r){
          var r = XN.JSON.parse(r.responseText);
          if(r.code == '0'){
            try{
              cFun();
            }catch(e){
              
            }
          }else{ //出错了 
          }
        }
      });
    }
    function doExchange(id, cFun){  //确认交换名片
      new XN.NET.xmlhttp({
        url: 'http://www.renren.com/address/acceptcard',  //2010-5-11 更改接口 参数ids -> id 只接受一个id
        data: 'id=' + id,
        onSuccess: function(r){
          var result = XN.JSON.parse(r.responseText);
          if(result.code == 0){
            if( cFun && isFunction(cFun) ){  //如果是外部调用,执行成功后返回他自己的处理逻辑
              cFun();
            }
          }else{ //出错了 
          }
        }
      });
    }

    function getCard(userid, cFun){  //获取对方名片信息 cFun(r)  r为返回的名片
      new XN.NET.xmlhttp({
        url: 'http://www.renren.com/showcard',
        method:'get',
        data: 'friendID=' + userid,
        onSuccess: function(r){
          var data = XN.JSON.parse(r.responseText);
          for(i in data){
            if(!data[i]){
              data[i] = undefined;
            }else if(data[i] == -1){
              data[i] = '<img src="http://a.xnimg.cn/imgpro/icons/newlock.png" title="隐私不可见" />';
            }
          }
          if( cFun && isFunction(cFun)){
            cFun( renderCard(data) );
          }else{
            XN.log('没有传入可用的cFun回调函数!');
          }
        }
      });
    }

    function renderCard(r){
      var HTML = '<a href="#" class="float-right"><img height="50" width="50" src="'+ r.head +'" class=""></a>'
             +'<h2>'+ r.name +'</h2>'
             +'<dl class="info-detail">'
             +' <dt>手机：</dt><dd>'+ (r.phone || '未填写') +'</dd>'
             +' <dt>QQ：</dt><dd>'+ (r.qq || '未填写') +'</dd>'
             +' <dt>MSN：</dt><dd>'+ (r.msn || '未填写') +'</dd>'
             +' <dt>邮箱：</dt><dd>'+ (r.email || '未填写') +'</dd>'
             +' <dt>地址：</dt><dd>'+ (r.address || '未填写') +'</dd>'
             +' <dt>补充：</dt><dd>'+ (r.bak || '未填写') +'</dd>'
             +'</dl>';
      return HTML;
    }

    var updateUI = function(witch, el){  //接受请求之后的显示状态 隐藏操作按钮
      var el = dom.wrap(el);
      var hostid = el.getData('host_id');
      var hostname = el.getData('host_name');
      if(witch == 'accept'){
        var HTML = '已经接受<a href="http://www.renren.com/profile.do?id='+ hostid +'" target="_blank"><strong> '+ hostname +' </strong></a>的交换请求，你能在第一时间收到TA资料更新的通知 '//<a href="javascript:;" onclick="violate.exchangeCard()">和其他好友交换</a> | '
            +'<a href="http://friend.renren.com/myfriendlistx.do#item_5"  target="_blank">查看我的通讯录 »</a>';
      }else if(witch == 'refuse'){
        var HTML = '已经拒绝<a href="http://www.renren.com/profile.do?id='+ hostid +'" target="_blank"><strong> '+ hostname +' </strong></a>的交换请求，TA不会收到你的资料更新通知 | '
            +'<a href="http://friend.renren.com/myfriendlistx.do#item_5"  target="_blank">查看我的通讯录 »</a>';
      }else if(witch == 'delete'){
        //直接删除这个条目
        el.dispose();
        return;
      }
      var detail = dom.getElement('div.detail', el);
      var btns = dom.getElement('div.btns', el);
      btns.hide();
      detail.innerHTML = HTML;
    };

    ReqCenter.regReqType('addr:259', {
      accept: function(info){
        var el = info.el; 
        var id = info.params[0];
        var nid = info.params[1];
        doExchange(id, function(){
          updateUI('accept', el);
          delNotify(nid);
        });
      },
      refuse: function(info){
        var id = info.params[0];
        var nid = info.params[1];
        var el = info.el;
        ignoreThese(id, function(){
          updateUI('refuse', el);
          delNotify(nid);
        });
      },
      showcard: function(info){
        //得先判断状态
        var el = info.el;
        var hostid = info.params;
        if(el.card){
          var card = el.card;
          card.style.display = card.style.display == 'none' ? 'block' : 'none';
        }else{
          var card = dom.getElement('div.cards-detail', el);
          el.card = card;
          getCard(hostid, function(html){
            el.card.innerHTML = html;
            el.card.style.display = '';
          });
        }
      },
      xxx: function(info){
        var id = info.params[0];
        var nid = info.params[1];
        var el = info.el;
        ignoreThese(id, function(){
          updateUI('delete', el);
          delNotify(nid);
        });
      }
    });
  });
</script>

<ul id="requests_addr_list" class="requests-list">
{{>LI_259}}
</ul>

{{#MORE}}
<div class="showmore">
  <a data-page="1" data-count="{{259_count}}" onclick="ReqCenter.funCall('req_more', 'addr', this)" class="requests-show-more">显示更多好友申请</a>
</div>
{{/MORE}}
