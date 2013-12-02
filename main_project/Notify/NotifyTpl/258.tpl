{{#HEAD}}
<div id="requests_tagPhoto_header" class="requests-header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/quan.gif">圈人请求<span>{{258_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=258&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
  function acceptPhotoRequest(id, ownerId, targetId, cFun){
    new XN.NET.xmlhttp({
      url:'http://photo.renren.com/acceptptrequest.do',
      data:'id='+id+'&ownerId='+ownerId+'&targetId='+targetId,
      onSuccess:function(r){
        var rs = XN.json.parse( r.responseText);
        var succ = rs.succ;
        if(succ){
          cFun(true);
        }else{
          XN.DO.showError( rs.msg );
          cFun(false);
        }
      },
      onError:function(){
        XN.DO.showError("操作失败，请稍后再试或者联系管理员");
      }
    });
  }
  // 拒绝圈人请求
  function refusePhotoRequest(id, ownerId, targetId, cFun){
    new XN.NET.xmlhttp({
      url:'http://photo.renren.com/refuseptrequest.do',
      data:'id='+id+'&ownerId='+ownerId+'&targetId='+targetId,
      onSuccess:function(r){
        cFun(true);
      },
      onError:function(){
        XN.DO.showError("操作失败，请稍后再试或者联系管理员");
      }
    });
  }
  ReqCenter.regReqType('tagPhoto:258', {
    accept: function(info){
      var el = dom.wrap(info.el);  //请求的li元素
      var requestId = info.params[0];
      var ownerId = info.params[1];
			var targetId= info.params[2];
      var nid = info.params[3];
      acceptPhotoRequest(requestId, ownerId, targetId, function(r){
        if(r){
          var photoId = el.getData('photo_id');
          var owner = el.getData('owner_id');
          var detail = dom.getElement('div.detail', el);  //准备改变这里的内容
          var btns = dom.getElement('div.btns', el);  //准备隐藏这些内容
          var photoLink = "<a href='http://photo.renren.com/getphoto.do?id="+photoId+"&owner="+owner+"'><strong> 查看这张照片 </strong></a>";
          var commentsLink = "<a href='http://photo.renren.com/getphoto.do?id="+photoId+"&owner="+owner+"&comments=true'><strong> 对照片发表评论 </strong></a>";
          detail.innerHTML="<p>接受圈人成功</p>"+"<p>你可以"+photoLink+"或者"+commentsLink+"</p>";
          btns.hide();
          delNotify(nid);
        }
      });
    },
    refuse: function(info){
      var el = dom.wrap(info.el);  //请求的li元素
      var requestId = info.params[0];
      var ownerId = info.params[1];
			var targetId= info.params[2];
      var nid = info.params[3];
      refusePhotoRequest(requestId, ownerId, targetId, function(r){
        if(r){
          var taggerId = el.getData('tagger_id');
          var taggerName = el.getData('tagger_name');
          var host = XN.user.id;
          var detail = dom.getElement('div.detail', el);  //准备改变这里的内容
          var btns = dom.getElement('div.btns', el);  //准备隐藏这些内容
          var personLink="<a href='http://www.renren.com/profile.do?id="+taggerId+"'><strong> "+taggerName+" </strong></a>";
          var albumLink="<a href='http://photo.renren.com/getalbumlist.do?id="+host+"'><strong> "+"进入相册"+"</strong></a>";
          var personalLink="<a href='http://www.renren.com/profile.do?id="+host+"'><strong> 返回我的个人主页 </strong></a>";
          detail.innerHTML="<p>拒绝了"+personLink+"的圈人请求</p>"+"<p>"+albumLink+"或者"+personalLink+"</p>";
          btns.hide();
          delNotify(nid);
        }
      });
    },
    xxx: function(info){
      var el = dom.wrap(info.el);
      var requestId = info.params[0];
      var ownerId = info.params[1];
			var targetId= info.params[2];
      var nid = info.params[3];
      refusePhotoRequest(requestId, ownerId, targetId, function(){
        el.style.background = '#FFFBC1';
        ReqCenter.FX.retract(el, function(){
          el.dispose();
          delNotify(nid);
        });
      });
    }
  });
      
	XN.dom.ready(function(){
		//模板里不能函数计算，所以只能拿出来算好再塞进去了，这是圈人的绝对定位
		var li = Sizzle('#requests_tagPhoto_list li');
		var picBox = Sizzle('#requests_tagPhoto_list li #picBox');
		var picName = Sizzle('#requests_tagPhoto_list li #picName');
		for(var i=0; i< li.length; i++){
			var leftToPhoto = li[i].getAttribute("data-leftToPhoto");
			var photoWidth = li[i].getAttribute("data-photoWidth");
			var topToPhoto = li[i].getAttribute("data-topToPhoto");
			var frameHeight = li[i].getAttribute("data-frameHeight");
			var frameWidth = li[i].getAttribute("data-frameWidth");
		
			var boxLeft = 200*leftToPhoto/photoWidth + 6;
			var boxTop = 200*topToPhoto/photoWidth + 6;
			var boxWidth = 200*frameWidth/photoWidth;
			var boxHeight = 200*frameHeight/photoWidth;
			var nameLeft = 200*leftToPhoto/photoWidth + 6;
			var nameTop = 200*topToPhoto/photoWidth + 200*frameHeight/photoWidth + 15;
			
			picBox[i].style.left = boxLeft + "px";
			picBox[i].style.top = boxTop + "px";
			picBox[i].style.width = boxWidth + "px";
			picBox[i].style.height = boxHeight + "px";
			picName[i].style.left = nameLeft + "px";
			picName[i].style.top = nameTop + "px";
		}
	}); 
	
}); 
</script>

<ul id="requests_tagPhoto_list" class="requests-list">
{{>LI_258}}
</ul>

{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{258_count}}" onclick="ReqCenter.funCall('req_more', 'tagPhoto', this)" class="requests-show-more">显示更多圈人请求</a>
</div>
{{/MORE}}
