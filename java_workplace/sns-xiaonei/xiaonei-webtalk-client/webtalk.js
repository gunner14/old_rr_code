var webtalkmanager = {};

/**
 * 在线好友列表展示用的窗口。模型是webtalkmanager.friendbox。
 * @param {Object} options
 */

webtalkmanager.friendbox = {

	//----------------- static ui ----------------------

	_content:function(){
		return ['<section class="webtalkmanager_friendbox">',
				'<h4>',
					'<span>在线好友</span>',
					'<span class="webtalkmanager_friendbox_fsearchinput">',
						'<input class="webtalkmanager_friendbox_fsearch" type="text" onsubmit="return false;" autocomplete="off" />',
					'</span>',
					'<span class="webtalkmanager_friendbox_fsearchreset" class="clear">',
						'<a href="#"><img src="http://a.xnimg.cn/n/core/res/webpager/clear.png" alt="清除" /></a>',
					'</span>',
				'</h4>',
				'<ul class="webtalkmanager_friendbox_flist">',
				'</ul>',
			'</section >'
		].join('');
	},

	//----------------- init ----------------------

	/*init的extend引入model
	model : webtalkmanager.friendlist,
	ctrl : webtalkmanager.ctrl
	*/

	init : function(options){
		$extend( this, options );//扩展this.model
		this.curValue = '';//当前搜索词
		this.initUI();//初始化ui,把所有元素装进自己的box中
		this.bindEvent();//绑定事件
		this.model.init();//friendlist在这里被初始化了
	},

	initUI : function(){
		var This = this;
		this.box = this.ctrl.tools.getDom( this._content() ).firstChild;
			this.h4 = this.box.getElementsByTagName( 'h4' )[0]; 
			this.span1 = this.h4.getElementsByTagName( 'span' )[1]; 
		this.fsearch = this.span1 = this.span1.getElementsByTagName( 'input' )[0];//搜索输入框 
		this.fsearchreset = this.h4.getElementsByTagName( 'span' )[2];//搜索重设叉叉
		$(this.fsearchreset).hide();//这个叉叉初始化时先隐藏
		this.flist = this.box.getElementsByTagName( 'ul' )[0];//好友列表容器
		this._searchHelper = XN.form.help( This.fsearch ).setDefaultValue('搜索好友');//搜索输入框灰色提示文字
	},

	//----------------- active ui ----------------------

	/**
	 * 拼每一个联系人的HTML结构
	 * @param {Object} obj - 表示一个好友的对象
	 */
	makePerson : function( obj ){
		return [
			'<li userid="', obj.userid ,'" username="', obj.username ,'" tinyurl="', obj.tinyurl,'" mainurl="0">',
				'<a userid="', obj.userid ,'" username="', obj.username ,'" tinyurl="', obj.tinyurl,'" mainurl="0">',
					'<span userid="', obj.userid ,'" username="', obj.username ,'" tinyurl="', obj.tinyurl,'" mainurl="0" class="webtalkmanager_friendbox_flist_li_img">',
						'<img src="">',
					'</span>',
					'<span userid="', obj.userid ,'" username="', obj.username ,'" tinyurl="', obj.tinyurl,'" mainurl="0">', obj.username ,'</span>',
					'<span userid="', obj.userid ,'" username="', obj.username ,'" tinyurl="', obj.tinyurl,'" mainurl="0" class="webtalkmanager_friendbox_flist_li_op">视频聊天</span>',
				'</a>',
			'</li>'
		].join('');
	},

	/**
	 * 用objs指定的好友列表的数组拼出整个HTML结构
	 * @param {Object} objs
	 */
	makePeople : function( objs ){
		var This = this;
		var people = [];
		XN.array.each( objs, function(i,obj){people.push( This.makePerson(obj) );});
		return people.join('');
	},

	/**
	 * friendlist内的好友列表发生改变后使用此方法更新UI
	 * @param {Object} objArr
	 */
	updateflist : function( objs ){
		this.flist.innerHTML = '';
		this.flist.innerHTML = this.makePeople( objs );
		imgs = this.flist.getElementsByTagName( 'img' );
		i=0;
		XN.array.each( objs, function(j,obj){
			imgs[i].src=obj.tinyurl;
			i++;
		});
	},

	//----------------- bind event ----------------------

	/**
	 * webtalkmanager.ctrl初始化完要显示好友列表时调用
	 */
	show : function(){
		$(this.box).show();
	},

	/**
	 * webtalkmanager.ctrl控制显示好友列表时调这货,显示后刷一次列表
	 */
	start : function(){
		$(this.box).show();
		this.model.queryonline();
	},

	/**
	 * webtalkmanager.ctrl控制显示单聊框而隐藏好友列表时调这货
	 */
	stop : function(){
		$(this.box).hide();
	},

	bindEvent : function(){
		var This = this;

		//----------------- this event ----------------------

		/**
		 * 搜索框有输入事件
		 */
		$(this.fsearch).addEvent( 'keyup', function(){
			if( This._searchHelper.getRealValue() != This.curValue ){
				This.curValue = This._searchHelper.getRealValue();//保存当前值
				This.fireEvent( 'webtalkmanager_friendbox_fsearch_search',This.curValue);
			}
		});

		$(this.fsearch).addEvent( 'keydown', function(e){
			e = e || window.event;
			if( e.keyCode == 38 ){//up
			}else if( e.keyCode == 40 ){//down
			}else if( e.keyCode == 13 ){//回车
				XN.event.stop(e);//回车要干掉默认事件, 不然会提交表单
			}
		});

		/**
		 * 真正处理搜索事件
	 	 * @param {string} curValue搜索框当前内容
		 */
		this.addEvent( 'webtalkmanager_friendbox_fsearch_search', function( curValue ){
			if( curValue ){
				$(This.fsearchreset).show();
			}else{
				$(This.fsearchreset).hide();
			}
			var found = This.model.search( curValue );
			This.updateflist( found );
		});

		/**
		 * 搜索重设按钮被点击事件
		 */
		$(this.fsearchreset).addEvent( 'click', function(){
			This.fsearch.value = '';
			This.fsearch.focus();
			This.curValue = '';//清空当前搜索值
			This.fireEvent( 'webtalkmanager_friendbox_fsearch_search',This.curValue);
		});

		/**
		 * 列表中的某人被点击了事件,e带着被点击的li,其中带有用户信息
		 */
		$(this.flist).addEvent( 'click', function( e ){
			This.fireEvent( 'webtalkmanager_friendbox_call', e );
		});

		//----------------- model event ----------------------

		/**
		 * 监听model的刷新完成事件
	 	 * @param {Object} obj 新得到的列表
		 */
		this.model.addEvent('webtalkmanager_friendlist_refresh', function( obj ){
			This.updateflist( obj );
		});

		/**
		 * 监听model的开始刷新事件
		 */
		this.model.addEvent('webtalkmanager_friendlist_refreshing', function(){
			This.flist.innerHTML = "<li>正在加载...</li>";
		});
	}

};
XN.event.enableCustomEvent( webtalkmanager.friendbox );

/**
 * webtalkmanager.friendbox的model: 好友列表
 */
webtalkmanager.friendlist = {
	map : {},
	init : function(){
		this.bindEvent();//绑定事件
	},
	bindEvent : function(){
		var This = this;
		this.addEvent('webtalkmanager_friendlist_queryonline', function(ret){
			var _map = {};
			XN.array.each( ret.friends, function(i,friend){
				_map[ friend.userid ] = friend;
			});
			This.map = _map;
			This.fireEvent( 'webtalkmanager_friendlist_refresh' , This.map );
		});
		this.addEvent('webtalkmanager_friendlist_queryonlineone', function(ret){
			This.map[ ret.friend.userid ] = ret.friend;
			This.fireEvent( 'webtalkmanager_friendlist_refresh' , This.map );
		});
	},
	queryonline : function( fn ){
		var This = this;
		this.fireEvent( 'webtalkmanager_friendlist_refreshing' );
		new XN.net.xmlhttp({
			url : 'http://webtalk.renren.com/webtalk/queryonline',
			method : 'post',
			data : 't='+Math.random(),
			onSuccess : function( r ){
				var ret = XN.json.parse( r.responseText );
				if( ret ){
					This.fireEvent( 'webtalkmanager_friendlist_queryonline' , ret);
				}
				if( fn ){ fn(); }
			}
		});
	},
	queryonlineone: function( userid, fn ){
		var This = this;
		new XN.net.xmlhttp({
			url : 'http://webtalk.renren.com/webtalk/queryonlineone',
			method : 'post',
			data : 'uid='+userid,
			onSuccess : function( r ){
				var ret = XN.json.parse( r.responseText );
				if( ret ){
					This.fireEvent( 'webtalkmanager_friendlist_queryonlineone', ret);
				}
				if( fn ){ fn(ret); }
			}
		});
	},
	search : function( key ){
		if( !key )return this.map;//如果要搜索空字符串, 就将整个friendlist返回给它
		var found = {};
		var username;
		XN.array.each( this.map, function(i,friend){
			username = friend.username;
			if( username.indexOf(key)!= -1 ){
				found[ friend.userid ] = friend;//将命中的记录保存起来
			}
		});
		return found;
	},
	gettinyurl : function( id ){
		return this.map[id]&&this.map[id].tinyurl;
	}
};
XN.event.enableCustomEvent( webtalkmanager.friendlist );

/**
 * 对话(单聊)窗口
 * @param {Object} options
 */
webtalkmanager.chatroom = function(options){
	$extend( this, options );
	this.init();
};

$extend(webtalkmanager.chatroom.prototype, {

	//----------------- static ui ----------------------

	_content:function(){
		return [
			'<div class="webtalkmanager_chatroom hbox">',
				'<div class="dialogs hbox">',
					'<article class="dialogs gspList">',
						this.getmsg(),
					'</article>',
				'</div>',
				'<div class="editor hbox">',
					'<form class="editor" action="" method="post">', 
						'<textarea></textarea>',
						'<input type="image" src="http://a.xnimg.cn/n/core/res/webpager/send-button.png" />',
					'</form>',
				'</div>',
			'</div>'
		].join('');
	},

	//----------------- init ----------------------

	/*构造函数的extend引入model
	model : new webtalkmanager.twoguychat,
	ctrl : webtalkmanager.ctrl
	*/

	init : function(){
		this.initUI();
		this.bindEvent();
	},

	initUI : function(){
		this.box = this.ctrl.tools.getDom( this._content() ).firstChild;
		$(this.box).hide();

		this.input = this.box.getElementsByTagName( 'textarea' )[0];
			this.form = this.box.getElementsByTagName( 'form' )[0];
			var inputs = this.form.getElementsByTagName( 'input' );
		this.sendbtn = $( inputs[inputs.length-1]);
		this.gspList = $( Sizzle('article.gspList',this.box )[0]);
		this.inputHelper = new XN.form.inputHelper(this.input); 
	},

	//----------------- active ui ----------------------

	/**
	 * 每一条对话的HTML通过这个方法拼装出来
	 * @param {Object} obj
	 */
	makeSection : function( obj ){
		var result = [];
		if( obj.fromuin == this.ctrl.local.userid ){
			srcHead = this.ctrl.local.tinyurl;
		}else{
			try{
				srcHead = webtalkmanager.friendlist.gettinyurl(obj.fromuin);
			}catch(e){
				//console.log(e);
			}
		}
		result.push(
			'<section>',
				'<figure class="avatar">',
					obj.type != 'system'?'<img width="35" height="35" src="' + srcHead + '" />':'',
				'</figure>',
				'<header style="text-align:left" >',
					'<span class="user">',
						'<a target="_blank" href="http://www.renren.com/profile.do?id='+ obj.fromuin +'">', obj.fromname,'</a>',
					'</span>',
					'<span class="time">', obj.time, '</span>',
				'</header>',
				'<section>',
					'<p style="text-align:left">',
						obj.type == 'system'? obj.msg_content:this.ctrl.tools.htmlFilter(obj.msg_content),//如果是系统消息就不需要过滤html,因为我需要用html来显示一些效果
					'</p>',
				'</section>',
				'<footer>',
				'</footer>',
			'</section>'
		);
		return result.join('');
	},

	//----------------- bind event ----------------------

	/**
	 * 事件绑定
	 */
	bindEvent : function(){
		var This = this;

		function send(){
			//console.log( '[send]begin' );
			if( XN.string.trim(This.inputHelper.getRealValue()) ){
				//调用模型完成一对一聊天业务
				//console.log( '[send]conv="' +This.inputHelper.getRealValue()+'" time="'+This.ctrl.tools.getTime()+'"' );
				var p = {
					name : This.ctrl.local.username,
					head : This.ctrl.local.tinyurl,
					msg_content : This.inputHelper.getRealValue(),
					time : This.ctrl.tools.getTime()
				};
				var ret = This.model.check( p.msg_content );
				if( !ret.isOK ){//验证不通过
					//console.log( '[send]check: bad conv' );
					This.model.sysmsg( ret.msg );
					//console.log( '[send]sysmsg: '+ret.msg );
					return;
				}
				//console.log( '[send]check: good conv broadcasting' );
				This.model.broadcast(p);
				//console.log( '[send]broadcast: done' );
				This.input.value = '';
			}
			This.input.blur();
			setTimeout(function(){
				This.input.focus();
			},10); 
		};

		//----------------- 界面事件--------------------------
		this.input.addEvent( 'keydown', function(e){
			e = e || window.event;
			if( e.keyCode == 13 ){
				send();
			}
		});

		this.sendbtn.addEvent( 'click', function(e){
			send();
			XN.event.stop(e);
		});

		// ------------------------------------------------ 监听模型事件------------------------------------------------------------
		this.model.addEvent( 'webtalkmanager_twoguychat_unshiftreply', function( conv ){
			try{
				/*
				//如果是fromuin 等于自己, 说明这条消息是服务器返回的、刚才我发送的那条消息
				if( conv.fromuin != this.ctrl.local.userid ){
					if( !This.model.isActive() || !This.active ){//如果不是当前打开的窗口就添加一个类在样式上来标识"收到一条新消息"
						XN.webpager.BlingMgr.titleBling.start();
						This.hlight();
					}	
				}
				*/
				This.addaconv( conv );
			}catch(e){
				//console.log(e);
			}
		});

		this.model.addEvent( 'webtalkmanager_twoguychat_replacereply', function(){
			This.updategsplist();
		});
	},

	start : function() {
		$(this.box).show();
	},

	stop : function() {
		$(this.box).hide();
	},

	//----------------- func ----------------------

	/**
	 * 拼接窗口的HTML片段
	 */
	getmsg : function(){
		var result = [];
		var replies = this.model.replies;
		for(var i=0; i<replies.length; i++){
			result.unshift(//不用push用unshift, 倒着来排, 最新的在最前面
				this.makeSection( replies[i] )	
			);
		} 
		return result.join('');		
	},

	/**
	 * 重新用model中的数据渲染对话列表
	 */
	updategsplist : function(){
		this.gspList.innerHTML += this.getmsg();
		this.gsplistscrollbtm();
	},
	/**
	 * 滚动条滚动到底部
	 */
	gsplistscrollbtm : function(){
		this.gspList.scrollTop = this.gspList.scrollHeight;
	},
	/**
	 * 添加并渲染一条对话
	 * @param {Object} conv
	 */
	addaconv : function( conv ){
		//EXP@huihua.lin: IE这个破浏览器不能在动态创建的元素上正确解析innerHTML进去的html5标签, 因此先调用getDom方法将html片段转化成为一个dom对象,然后append
		if( XN.browser.IE ){
			try{
				this.gspList.appendChild( this.ctrl.tools.getDom(this.makeSection(conv)) );
			}catch(e){
				//console.log(e);
			}
		}else{//标准浏览器就直接使用innerHTML方法
			try{
				this.gspList.appendChild( this.ctrl.tools.getDom(this.makeSection(conv)) );
			}catch(e){
				//console.log(e);
			}
		}
		this.gsplistscrollbtm();
	}

});
XN.event.enableCustomEvent( webtalkmanager.chatroom.prototype );

/**
 * 单聊的聊天室model
 */
webtalkmanager.twoguychat = function(options){
	$extend( this, options );
};

$extend(webtalkmanager.twoguychat.prototype, {

	/*
	replies:[],
	userid : x,
	username : x,
	tinyurl : x,
	ctrl : webtalkmanager.ctrl
	*/

	/**
	 * 替换整个聊天列表。载入历史聊天时有用
	 * @param {Object} rs
	 */
	replacereplies : function( rs ){//rs means replies
		this.replies = rs;
		this.fireEvent('webtalkmanager_twoguychat_replacereply');
	},
	/**
	 * twoguychat.prototype.check
	 * 单聊内容不能超过2000个字
	 * @param {Object} str
	 */
	check : function( str ){
		if( str.length > 2000 ){
			return { isOK:false, msg:this.ctrl.msg.TOO_LONG };
		}
		return { isOK : true, msg : 'sus' };
	},
	/**
	 * twoguychat.prototype.broadcast
	 * 发送聊天内容
	 * @param {Object} conv
	 */
	broadcast : function( conv ){
		conv.msg_content = this.ctrl.tools.htmlFilter( conv.msg_content );
		conv.fromuin = this.ctrl.local.userid;
		conv.fromname = this.ctrl.local.username;
		conv.touin = this.userid; 
		conv.msg_content = this.ctrl.tools.unescapeHTML(conv.msg_content);
		webpager.sendMessage( conv );
	},
	/**
	 * twoguychat
	 * 添加一个reply, 并fire相应的事件, 通知view, 说model改变了
	 * @param {Object} conv
	 */
	unshiftreply : function( conv ){
		this.replies.unshift( conv ); 
		this.fireEvent('webtalkmanager_twoguychat_unshiftreply', conv);
	},
	/**
	 * 在单聊窗口中显示一条系统消息
	 * @param {Object} msg
	 */
	sysmsg : function( msg ){
		var p = {
			fromname : '',
			head : this.ctrl.local.tinyurl,
			msg_content : msg,
			time : this.ctrl.tools.getTime(),
			type : 'system'
		};
		this.unshiftreply( p );
	},

	/**
	 * 使用这个方法随时更新单聊对象现在的头像
	 * @param {Object} obj
	 */
	updateprofile: function( username,tinyurl ){
		this.username = username;
		this.tinyurl = tinyurl;
	}

});
XN.event.enableCustomEvent( webtalkmanager.twoguychat.prototype );

/**
 * 一个工具类。按照一定的时间间隔，交替运行定义的两个函数。
 * 用来实现title和提醒的闪动效果
 * @param {Object} xw
 */
 
webtalkmanager.bling = {
	onStart : function(){
		if( document.title.indexOf('【')!=-1 ){
			return; 
		}
		this.oldtitle = document.title;
		window.imengine.imHelper.playSound();
	},
	fn1 : function(){
		document.title = '【新来电】- ' + this.oldtitle;                   
	},
	fn0 : function(){
		document.title = '【　　　】- ' + this.oldtitle;
	},
	start : function(){
		var This = this;
		var i = 0;
		if( this.inter ){
			clearInterval( this.inter );
		}
		if( this.onStart ){
			this.onStart();
		}
	
		this.inter = setInterval( function(){
			if( i++%2 ){
				This.fn1();
			}else{
				This.fn0();
			}
		},this.timeInterv||1000);
	},
	stop : function(){
		if( this.inter ){
			clearInterval( this.inter );
		}
		document.title = this.oldtitle;
	}
};

/**
 * 对话管理器, 接受到消息,分析消息的类型并将消息分发,消息包括: 新来电,新到消息
 */
webtalkmanager.ctrl = function(options){
	$extend( this, options );
	this.init();
};
$extend(webtalkmanager.ctrl.prototype, {

	status : {
		talking : false
	},

	url : {
		OFFLINE_IMG : 'http://head.xiaonei.com/photos/0/0/men_tiny.gif'
	},

	msg : {
		INPUT_DEFAULT : '请输入聊天内容',
		TOO_LONG : '您发送的内容超过2000字, 发送失败',
		NOTICE : '为保护你的隐私，将默认此对话为悄悄话！',
		SPAM : {
			'-1' : '您目前不在登录状态，请尝试重新登录后再试。如果您一直收到这个信息，请联系管理员',
			2 : '该状态不存在',
			4 : '请不要发布政治敏感内容、色情内容、商业广告或其他不恰当内容',
			5 : '你短时间内发表了太多相同的内容'
		}
	},

	tools : {
		htmlFilter : function( str ){
			str = str.replace(/&/g,'&amp;');
			str = str.replace(/</g,'&lt;');
			str = str.replace(/>/g,'&gt;');
			return str;
		},
		getTime : function( d ){
			var d = d || new Date();
			return this.two((d.getMonth()+1)) +'-'+ this.two(d.getDate()) +' '+ this.two(d.getHours()) +':'+ this.two(d.getMinutes());
		},
		two : function(s){
			if( parseInt(s) > 9 )return s;
			return '0'+s;
    		},
		getDom : function(str) {
			var tmp = document.createElement('div');
			tmp.style.display = 'none';
			document.body.appendChild(tmp);

			tmp.innerHTML = str;
			//var dom = document.createElement('div');
			var dom = document.createDocumentFragment();
			while (tmp.firstChild) dom.appendChild(tmp.firstChild);
			tmp.parentNode.removeChild(tmp);
			return dom;
		},
		unescapeHTML : function(html) {
			var n = document.createElement("div");
			n.innerHTML = html;
			if(XN.browser.IE){
				return n.innerText;	 
			}else{
				return n.textContent; 
			}
		}
	},

	rooms : {},
	activeroom : null,

	/*构造函数的extend引入local和friendbox
	local : {
		userid : x,
		username : x,
		tinyurl : x
	},
	friendbox : webtalkmanager.friendbox,
	*/

	init : function(){
		this.bindEvent();
		this.container = $( 'webtalk_main_op' );
		this.friendbox.init({
			model : webtalkmanager.friendlist,
			ctrl : this
		})
		this.container.appendChild( this.friendbox.box );
		this.showfriendbox(false); 
	},

	bindEvent : function(){
		var This = this;

		// ----------------- 监听来自pager-channel的事件 ----------------------

		// ----------------- 单聊消息 ------------------------------ //
		webpager.addEvent('message_got',function(m){
			if( m.r_id ){ //收到群聊的消息
			}else{ //收到单聊消息
				This.twoguymsg(m);
			}
		});
		// ----------------- webpager断开与服务器的连接(有可能是用户退出或者网络问题等等原因) -------------------------- //
		webpager.addEvent('channel_disconnected',function(){
			//This.showfriendbox(false);
			//location.href='http://www.renren.com/Logout.do?get_check='+This.local.userid;
		});
		this.friendbox.addEvent('webtalkmanager_friendbox_call',function(e){
			e = e || window.event;
			var ta = e.srcElement || e.target;
			if(!ta.getAttribute('userid'))return;
			var _userid = ta.getAttribute('userid');
			var _username = ta.getAttribute('username');
			var _tinyurl = ta.getAttribute('tinyurl');
			var _mainurl = ta.getAttribute('mainurl');
			webtalkConnecting( _userid,_username,_tinyurl,_mainurl );
		});
	},

	twoguymsg : function(msgObj){
		var This = this;
		var p = {};
		if( this.local.userid == msgObj.fromuin ){
			p.userid = msgObj.touin;
		}else{
			p.userid = msgObj.fromuin;
		}
		var fri = webtalkmanager.friendlist.map[ p.userid ]; //通过fromuin在好友列表里面查找好友数据
		msgObj.time = this.tools.getTime();//用当前的即时时间
		try{
			//如果这条消息的发送者不在当前的在线好友列表里面,需要重新load一下好友列表.因为没有好友列表就取不到消息发送者的头像等信息
			if( !fri ){
				//发送请求到好友业务获取好友信息
				p.username = '';
				p.tinyurl = this.url.OFFLINE_IMG;
				var room = this.getroom ( p );
				webtalkmanager.friendlist.queryonlineone(msgObj.fromuin,function(obj){
					room.model.updateprofile(ret.friend.username,ret.friend.tinyurl);
					room.model.unshiftreply(msgObj);
				});
			}else{
				p.username = fri.username;
				p.tinyurl = fri.tinyurl;
				var room = this.getroom ( p );
				room.model.unshiftreply( msgObj );//改变数据, 然后unshiftreply就会自己通知view做出相应的改变
			}
		}catch(e){
			//console.log(e);
		}
	},

	getroom : function( userObj ){
		if( !userObj ) return null;
		var room = this.rooms[ userObj.userid ]; //看看用这个userObj里面的数据能不能获得一个已经存在的Conv实例
		if( room ){//如果这个Conv已经存在于展示出来的那些窗口所组成的集合中, 就不用再add了
			return this.rooms[ userObj.userid ];
		}
		var m = new webtalkmanager.twoguychat({
			replies:[],
			userid : userObj.userid,//目标用户的userid , 这个userid将会被作为索引对话实例的键值(key)
			username : userObj.username,
			tinyurl : userObj.tinyurl,
			ctrl : this
		});
		var v = new webtalkmanager.chatroom({
			model : m,
			ctrl : this
		});
		this.container.appendChild( v.box );
		this.rooms[ userObj.userid ] = v;//添加进当前展示窗口的集合中,记录下来以便以后索引
		return v; 
	},

	showchatroom : function( _userid,_username,_tinyurl ){ 
		var p = {
			userid : _userid,
			username : _username,
			tinyurl : _tinyurl
		};
		var room = this.getroom( p );
		this.friendbox.stop();
		if( this.activeroom ){
			this.activeroom.stop();
			this.activeroom = null;
		}
		this.activeroom = room;
		this.activeroom.start();
		this.status.talking = true;
	},

	showfriendbox : function(tostart){
		if( this.activeroom ){
			this.activeroom.stop();
		}
		this.activeroom = null;
		if( tostart ){
			this.friendbox.start();
		}else{
			this.friendbox.show();
		}
		this.status.talking = false;
	},
	
	startbling : function(){
		if( this.bling ){
			this.bling.start();
		}
	},
	
	stopbling : function(){
		if( this.bling ){
			this.bling.stop();
		}
	}
	

});
XN.event.enableCustomEvent( webtalkmanager.ctrl.prototype );

