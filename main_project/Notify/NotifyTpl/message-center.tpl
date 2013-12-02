		<!-- 左侧导航菜单 --> 
		<div id="subNav" class="sub-nav"> 
			<h1 class="page-title">消息中心</h1>
			
			<div class="class-title"><a click="remind:all">提醒</a></div>
			<ul class="request-menu"> 
       {{#FLAG_REMIND}}
				<li>
					<a href="#" click="remind:leave"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/leave.gif" width="16" height="16">留言回复提醒<span class="count">{{REMIND}}</span></a> 
				</li>
       {{/FLAG_REMIND}}
				<!--
				<li>
					<a href="http://msg.renren.com/" target="_blank"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/game.png" width="16" height="16">站内信</a> 
				</li>
				-->
			</ul>
			
			<div class="class-title"><a click="friend:all" class="select">请求</a></div>
			<ul class="request-menu"> 
        {{#FLAG_256}}
				<li>
					<a href="#" click="friend:friend"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/friend.gif" width="16" height="16">好友请求<span class="count">{{256_count}}</span></a>
				</li>
        {{/FLAG_256}}
        {{#FLAG_260}}
				<li>
					<a href="#" click="friend:tuijian"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/friend.gif" width="16" height="16">好友推荐<span class="count">{{260_count}}</span></a>
				</li>
        {{/FLAG_260}}
        {{#FLAG_258}}
				<li>
					<a href="#" click="friend:tagPhoto"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/quan.gif" width="16" height="16">圈人请求<span class="count">{{258_count}}</span></a>
				</li>
        {{/FLAG_258}}
        {{#FLAG_259}}
				<li>
					<a href="#" click="friend:addr"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/addressbook.gif" width="16" height="16">通讯录请求<span class="count">{{259_count}}</span></a>
				</li>
        {{/FLAG_259}}
        {{#FLAG_262}}
				<li>
					<a href="#" click="friend:appmessage"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/app.png" width="16" height="16">应用请求<span class="count">{{262_count}}</span></a>
				</li>
        {{/FLAG_262}}
        {{#FLAG_373}}
				<li>
					<a href="#" click="friend:apppage"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/app.png" width="16" height="16">应用关联请求<span class="count">{{373_count}}</span></a>
				</li>
        {{/FLAG_373}}
        {{#FLAG_265}}
				<li>
					<a href="#" click="friend:xiaozuApply"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/group.gif" width="16" height="16">小组申请<span class="count">{{265_count}}</span></a>
				</li>
        {{/FLAG_265}}
        {{#FLAG_261}}
				<li>
					<a href="#" click="friend:xiaozuInvite"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/group.gif" width="16" height="16">小组邀请<span class="count">{{261_count}}</span></a>
				</li>
        {{/FLAG_261}}
        {{#FLAG_264}}
				<li>
					<a href="#" click="friend:pageInvite"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/page.gif" width="16" height="16">主页推荐<span class="count">{{264_count}}</span></a>
				</li>
        {{/FLAG_264}}
        {{#FLAG_266}}
				<li>
					<a href="#" click="friend:page"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/page.gif" width="16" height="16">主页管理员邀请<span class="count">{{266_count}}</span></a>
				</li>
        {{/FLAG_266}}
        {{#FLAG_247}}
				<li>
					<a href="#" click="friend:lover"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/lovehouse.gif" width="16" height="16">情侣空间邀请<span class="count">{{247_count}}</span></a>
				</li>
        {{/FLAG_247}}
        {{#FLAG_279}}
				<li>
					<a href="#" click="friend:xiaozhan"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/zhan.gif" width="16" height="16">小站推荐请求<span class="count">{{279_count}}</span></a>
				</li>
        {{/FLAG_279}}
        {{#FLAG_299}}
				<li>
					<a href="#" click="friend:xzInvite"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/zhan.gif" width="16" height="16">小站关注请求<span class="count">{{279_count}}</span></a>
				</li>
        {{/FLAG_299}}
        {{#FLAG_325}}
				<li>
					<a href="#" click="friend:showLove"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/lovehouse.gif" width="16" height="16">感情状态请求<span class="count">{{325_count}}</span></a>
				</li>
        {{/FLAG_325}}
			</ul>


			<!-- 通知 -->
			<div class="class-title"><a click="notice:all">通知</a></div>
			<ul class="request-menu">
				{{#FLAG_NOTICE}}
				<li>  
					<a href="#" click="notice:notice"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/hi.gif" width="16" height="16">通知<span class="count">{{NOTICE_NUM}}</span></a> 
				</li>
				{{/FLAG_NOTICE}}
				{{#FLAG_HI}}
				<li> 
					<a href="#" click="notice:poke"><img class="icon" src="http://a.xnimg.cn/imgpro/v6/icon/hi.gif" width="16" height="16">招呼<span class="count">{{278_count}}</span></a> 
				</li> 
				{{/FLAG_HI}}
			</ul>
			
			
		</div>
		
		<!-- 右侧请求列表 --> 
		<div id="scrollBox" class="requests-detail"> 
			<div class="container" id="scrollContainer"> 

				<div id="list_remind" style="display:none;">
					{{>UL_MESSAGE}}
					{{>REMIND_MAIL}}
					{{>NOREMIND_DEAL}}
				</div>
				<div id="list_friend" style="display:none;">
					{{>UL_256}}
					{{>UL_260}}
					{{>UL_258}}
					{{>UL_259}}
					{{>UL_262}}
					{{>UL_373}}
					{{>UL_265}}
					{{>UL_261}}
					{{>UL_264}}
					{{>UL_266}}
					{{>UL_247}}
					{{>UL_279}}
					{{>UL_299}}
					{{>UL_325}}
					{{>NOAPPLY_DEAL}}
				</div>
				<div id="list_notice" style="display:none;">
					{{>NOTICE_GAMES}}
					{{>NOTICE_VIP}}
					{{>NOHI_DEAL}}
					{{>UL_NOTICE}}
				</div>
			</div> 
			<div style="height: 0px"></div>
		</div> 
