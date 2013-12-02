{{#HEAD}}
<div id="requests_friend_header" class="requests-header">
  <h2><img src="http://a.xnimg.cn/imgpro/v6/icon/friend.gif">好友请求<span>{{256_count}}</span></h2>
</div>
{{/HEAD}}
<ul id="requests_friend_list" class="requests-list">
{{>LI_256}}
</ul>

<script type="text/javascript">
object.use('dom, net', function(exports, dom, net){
	//删除notify
	function delNotify(nid){
		new XN.NET.xmlhttp({
			url:'http://notify.renren.com/rmessage/remove?type=256&nid=' + nid,
			method:'post',
			onSuccess:function(r){
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});	
	}
	//接受好友邀请后显示UGC内容
	function getUgcPhoto(id){
		if( !$('friend_ugc_' + id) ) return;	
		var callFun = function(list){
			if( list.length == 0 ) return;
			if( !$('friend_ugc_' + id) ) return;
			var str = '<dt>最新照片：</dt>'
		
			for(var i= 0 ; i < list.length ; i++ ){
				var obj = list[i];
				str += '<dd><a target="_blank" href="'+obj.src +'?href=1010"><img src="'+obj.img+'" width="50" height="50" /></a></dd>'
			}
			str += '<dd><a target="_blank" href="http://photo.'+XN.env.domain+'/getalbumlist.do?id='+id+'&href=1010" class="morealbums">更多相册...</a></dd>'
			
			$('friend_ugc_photo_' + id).innerHTML = str;
			$('friend_ugc_photo_' + id).show();
			$('friend_ugc_' + id).show();
		}
		
		new XN.NET.xmlhttp({
			url : 'http://photo.'+XN.env.domain+'/photo/'+id+'/recommendPhotosForGuest',
			onSuccess: function(r){
				try {
					var json = XN.JSON.parse(r.responseText);
				}catch(e){
					return;
				}
				callFun( json || [])
			}
		})
		//TODO 发送请求	
	}
	function getUgcState(id){
		if( !$('friend_ugc_' + id) ) return;
		var callFun = function(obj){
			if( !obj || !obj.doingCount ||  !obj.content) return;
			if( !$('friend_ugc_' + id) ) return;
			var str = '<dt>最新状态：</dt>'
			str += '<dd class="request-feed">'+obj.content+'<span class="updatetime">'+obj.dtime+'</span> <a target="_blank" href="http://status.'+XN.env.domain+'/status/'+id+'/'+obj.id+'/">回复</a><span class="pipe">|</span><a target="_blank" href="http://status.'+XN.env.domain+'/getdoing.do?id='+id+'">查看更多</a></dd>'
			$('friend_ugc_state_' + id).innerHTML = str;
			$('friend_ugc_state_' + id).show();
			$('friend_ugc_' + id).show();
		}
		
		new XN.NET.xmlhttp({
			url : 'http://status.'+XN.env.domain+'/status/currentDoing?userId=' + id,
			onSuccess: function(r){
				try {
					var json = XN.JSON.parse(r.responseText);
				}catch(e){
					return;
				}
				callFun( json )
			}
		})
		
	}

	//TODO 增加显示相册和状态的地方
	function getAcceptMessage(id,response,name){			
		if(response.ret==1){
			return ['<p class="confirm">已经加<a href="http://www.' + XN.env.domain + '/profile.do?id=',
				id,
				'"><strong> ',
				name,
				' </strong></a>为好友,你可以: <strong><a href="http://www.' + XN.env.domain + '/profile.do?id=',
				id,
				'">查看TA的个人主页</a></strong><strong style="margin-left:10px;"><a href="http://gossip.' + XN.env.domain + '/getgossiplist.do?id=',
				id,
				'&age=recent">给TA留言</a></strong><strong style="margin-left:10px;"><a href="http://msg.' + XN.env.domain + '/SendMessage.do?id=',
				id,
				'">给TA发站内信</a></strong></p>','<div style="display:none;" id="friend_ugc_'+id+'" class="req-recommend"><dl style="display:none;" id="friend_ugc_photo_'+id+'" class="req-recommend-photo clearfix"></dl><dl style="display:none;" id="friend_ugc_state_'+id+'" class="clearfix"></dl></div>'].join("");
		}
		else{
			return getFailMessage(response.message);
		}			
	}
	// 好友申请的接受 忽略和我不认识TA
	function fillGroup(id,dialog){
		new XN.NET.xmlhttp({
			url:'http://friend.' + XN.env.domain + '/showfriendgroup.do',
			data:"friendId=" + id,
			onSuccess:function(r){
				dialog.body.setContent(r.responseText);
			},
			onError:function(){
				dialog.hide();
				XN.DO.showError('网络通信失败,请重试');
			},
			onComplete:function(){
				if(dialog!=null)
					dialog.footer.show();
			}
		});
	}
	function getAcceptMessage(id,name){			
		return ['<p class="confirm">已经加<a href="http://www.' + XN.env.domain + '/profile.do?id=',
			id,
			'"><strong> ',
			name,
			' </strong></a>为好友,你可以: <strong><a href="http://www.' + XN.env.domain + '/profile.do?id=',
			id,
			'">查看TA的个人主页</a></strong><strong style="margin-left:10px;"><a href="http://gossip.' + XN.env.domain + '/getgossiplist.do?id=',
			id,
			'&age=recent">给TA留言</a></strong><strong style="margin-left:10px;"><a href="http://msg.' + XN.env.domain + '/SendMessage.do?id=',
			id,
			'">给TA发站内信</a></strong></p>','<div style="display:none;" id="friend_ugc_'+id+'" class="req-recommend"><dl style="display:none;" id="friend_ugc_photo_'+id+'" class="req-recommend-photo clearfix"></dl><dl style="display:none;" id="friend_ugc_state_'+id+'" class="clearfix"></dl></div>'].join("");
	}
	function acceptFriend(id, nid, cFun){
		var dialog = XN.DO.confirm({
			title:'接受好友请求',
			msg:'<center><img src="'+XN.ENV.staticRoot+'img/indicator.gif" />加载中...</center>',
			callBack:function(r){
				if(r){
					var group=$("popFriendGroups").getElementsByTagName("input");
					var params={action:"addmultgroups",buddys:id,names:[]};
					
					for(var i=0;i<group.length;i++){
						if(group[i].checked){
							params.names.push(group[i].value);
						}
					}

					new XN.NET.xmlhttp({
						url:'http://friend.' + XN.env.domain + '/ApplyGuestRequest.do',
						data:"friendId=" + id + "&post="+ encodeURIComponent(XN.JSON.build(params)),
						onSuccess:function(r){
							var response=XN.JSON.parse(r.responseText);
							if(response.ret == 1){
								cFun();
								delNotify(nid);
							}else if(response.ret == 5){
								dialog = new XN.ui.dialog({
									title : '无法添加好友',
									message: ['<p>你的好友数已到上限，可以通过<a style="font-weight:bold;" href="http://i.renren.com" target="_blank">VIP</a>服务提高好友数上限，或者删除部分好友后重试。</p>'].join(''),
									showCloseButton: true
								});
								dialog.footer.hide();
								return;
							}else if(response.ret == 4){
								var ta = (response.ta == '女生') ? '她' : '他';
								dialog = new XN.ui.dialog({
									title : '无法添加'+ response.applicantName +'为好友',
									message: ['<p><a style="font-weight:bold;" href="http://www.renren.com/profile.do?id='+ response.applicantId +'"  target="_blank">'+ response.applicantName +'</a>好友数已到上限，可以送'+ ta +'<a style="font-weight:bold;" href="http://i.renren.com" target="_blank">VIP</a>提高'+ ta +'的好友数上限。</p>'].join(''),
									showCloseButton: true
								});
								dialog.footer.hide();
								return;
							} else {
								XN.DO.showError( '<span style="color:#FF0000;">'+(response.message || "操作失败")+'</span>' );
							}
						},
						onError:function(){
							XN.DO.showError('网络通信失败,请重试');
						}
					});
				}
			}
		});
		dialog.footer.hide();
		fillGroup(id,dialog);
	}
	// 拒绝好友请求
	function rejectFriend(id, nid, cFun){
		new XN.NET.xmlhttp({
			url:'http://friend.' + XN.env.domain + '/rejguereq.do',
			data:"id="+id,
			onSuccess:function(r){
				cFun();
				delNotify(nid);
			},
			onError:function(){
				XN.DO.showError('网络通信失败,请重试');
			}
		});			
	}
	// 忽略好友请求
	function ignoreFriend(e,id,name,type){
		var dialog = XN.DO.confirm({
			title:('提示'),
			msg:'<div style="border: 2px solid #FFFFFF;padding:4px;background: rgb(254, 255, 207) none repeat scroll 0% 0%;color: rgb(239, 66, 35);">' + XN.env.siteName + '是你与熟人之间的社交网络，建议勿加陌生人为好友！</div><div class="clearfix" style="font-size: 14px; line-height: 1.8em; padding-left: 20px;"><h3 style="background: transparent url(http://s.xnimg.cn/imgpro/icons/attention2.gif) no-repeat scroll 0pt center;padding-left: 20px;">加陌生人为好友可能的不良后果:</h3><ol style="margin-left: 40px;"><li>泄露你的个人隐私</li><li>被陌生人骚扰</li></ol></div>',
			yes:"忽略此好友请求",
			callBack:function(r){
				if(r)
					rejectFriend(e,id,name,type);
			}
		});
	}
	function getInfoBox(el){
		//var div_info = dom.getElement('div.info-box', el);
		var div_share = dom.getElement('div.info-box', el);

		return {
			//info: div_info, 
			share: div_share
		};
	}
	function loadFriendInfo(id, el){
		//发请求,取用户资料 拼接 填充到el中
		var xhr = new net.Request({
			url: 'http://www.renren.com/ajax_datum',
			method: 'get',
			onSuccess: function(r){
				var r = JSON.parse(r.responseText);
				//
			}
		});
		xhr.send();
	}
	function initFriend(params){
		new XN.NET.xmlhttp({
			url: 'http://friend.renren.com/j_f_cfb?guests='+ params,
			method: 'post',
			onSuccess:function(r){
				var r = XN.JSON.parse(r.responseText);
				for( var id in r){
					var num = r[id].commonFriendCount;
					if(num > 0){
						var description = $('description_'+ id);
						description.appendChild(showShareLI(num, id));
						
						var showShare_ul = $('showShare_ul_'+ id);
						var fr = r[id].commonFriendList;
						fr.forEach(function(f, i){
							showShare_ul.appendChild(showShareUI(f));
						});
					}
				}
			},
			onError:function(){
				XN.DO.showError('操作失败，请联系管理员或者稍后再试');
			}
		});
	}
	//显示有几个共同好友
	function showShareLI(num, params){
		var div = document.createElement('div');
		var HTML = ['	<p><a click="friend_showShare:'+ num +'" href="#nogo">你们拥有'+ num +'个共同好友</a></p>'+
								'	<div class="info-box" id="info_box" style="display:none">'+
								'		<span class="pop-arrow bothfriend-arrow"></span>'+
								'		<ul id="showShare_ul_'+ params +'">'+
								'		</ul>'+
								'	</div>'].join();
		div.innerHTML = HTML;
		return div;
	}
	//显示共同好友列表
	function showShareUI(r){
		var Li = document.createElement('li');
		var HTML = ['	<a class="name" href="http://www.renren.com/profile.do?id='+ r.c_id +'"  target="_blank">'+
								'		<img class="avatar" src='+ r.c_head +'>'+ r.c_name +
								'	</a>'].join();
		Li.innerHTML = HTML;
		return Li;
	}
	ReqCenter.regReqType('friend:256', {
		accept: function(info){
			var el = dom.wrap( info.el );
			var id = info.params[0];
			var name = info.params[1];
			var nid = info.params[2];
		
			acceptFriend(id, nid, function(){
				// 计算分页偏移量
				//adjustOffset(type);
				var btns = dom.getElement('div.btns', el);
				var detail = dom.getElement('div.detail', el);

				detail.innerHTML = getAcceptMessage(id,name);
				btns.hide();
				//XN.APP.requestCenter.fireEvent( 'friend_request_accepted', response, id );
			});
		},
		refuse: function(info){
			var el = dom.wrap( info.el );
			var id = info.params[0];
			var name = info.params[1];
			var nid = info.params[2];
			
			rejectFriend(id, nid, function(){
				var btns = dom.getElement('div.btns', el);
				var detail = dom.getElement('div.detail', el);
				
				detail.innerHTML="已经拒绝<a href='http://www." + XN.env.domain + "/profile.do?id="+id+"'><strong> "+name+" </strong></a>的好友请求,你可以进行<a href='http://www." + XN.env.domain + "/privacyhome.do'><strong> 隐私设置 </strong></a>以减少陌生人的骚扰";
				btns.hide();
			});
			// 计算分页偏移量
			//adjustOffset(type);
		},
		dontknow: function(info){
			var dialog = XN.DO.confirm({
				title:('提示'),
				msg:'<div style="border: 2px solid #FFFFFF;padding:4px;background: rgb(254, 255, 207) none repeat scroll 0% 0%;color: rgb(239, 66, 35);">' + XN.env.siteName + '是你与熟人之间的社交网络，建议勿加陌生人为好友！</div><div class="clearfix" style="font-size: 14px; line-height: 1.8em; padding-left: 20px;"><h3 style="background: transparent url(http://s.xnimg.cn/imgpro/icons/attention2.gif) no-repeat scroll 0pt center;padding-left: 20px;">加陌生人为好友可能的不良后果:</h3><ol style="margin-left: 40px;"><li>泄露你的个人隐私</li><li>被陌生人骚扰</li></ol></div>',
				yes:"忽略此好友请求",
				callBack:function(r){
				if(r)
					//rejectFriend(id);
					ReqCenter.funCall('friend_refuse', info.params.join(), info.el);
				}
			});
		},
		xxx: function(info){
			var el = dom.wrap(info.el);
			var id = info.params[0];
			var nid = info.params[1];
			rejectFriend(id, nid, function(){
				el.dispose();
			});
		},
		ignoreAll: function(info){
			var rt = confirm("执行这个操作会删除所有"+name+"，删除后不能恢复，你确定删除吗？"); 
			if(rt){
				new XN.NET.xmlhttp({
					url: 'http://friend.renren.com/delallguestrequest.do?id='+ XN.user.id,
					method: 'post',
					onSuccess:function(r){
						var req_list = $('requests_friend_list');
						req_list.innerHTML = '<li class="request">成功忽略所有好友申请</li>';
					},
					onError:function(){
						XN.DO.showError('操作失败，请联系管理员或者稍后再试');
					}
				});
			}
		},
		showInfo: function(info){
			//显示用户资料 跟共同好友互斥
			var el = dom.wrap(info.el);
			var id = info.params;
			
			var divs = getInfoBox(el);
			divs.share.hide();
			divs.info.toggle();
			if(!divs.info.loaded){
				loadFriendInfo(id, divs.info);
			}
		},
		showShare: function(info){
			if(info.params == 0) return; //没有共同好友
			//现实共同好友 跟用户资料互斥
			var el = dom.wrap(info.el);
			var divs = getInfoBox(el);

			//divs.info.hide();
			divs.share.toggle();
		},
		
		//@param info {
		//		action : 'more', 
		//		el: 被点击的那个“更多”按钮, 
		//		params: 刚刚加载的第几页, 
		//		type: 当前的请求类型，没有用
		//	}
		more: function(info){
				var list = info.params[1];
				var params = [];
				
				for(var i=0; i< list.length; i++){
					var a = Sizzle('.figure a', list[i])[0];
					var id = a.href.split('=')[1];
					params[i]= id;
				}
				initFriend(params);
		}
	});
	
	XN.dom.ready(function(){
		var params = [];
		var a = Sizzle('#requests_friend_list li.request .figure a');
		for(var i=0; i< a.length; i++){
			var id = a[i].href.split('=')[1];
			params[i]= id;
		}
		initFriend(params);
	});
});	
</script>


{{#MORE}}
<div class="showmore">
  <a data-page="1" data-count="{{256_count}}" onclick="ReqCenter.funCall('req_more', 'friend', this)" class="requests-show-more">显示更多好友申请
</a>
</div>
{{/MORE}}



