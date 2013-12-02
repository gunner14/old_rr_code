{{#LI_SEC}}
	<li class="request">
		<div class="figure">
			<a href="http://www.renren.com/profile.do?id={{userId}}" target="_blank">
				<img class="" height="50" width="50" src="{{userHeadImag}}">
			</a>
		</div>
		<div class="section">
			<img src="http://a.xnimg.cn/imgpro/icons/invite-prd.png"> <a href="http://www.renren.com/profile.do?id={{userId}}" target="_blank">{{userName}}</a> 邀请你成为 <a href="http://page.renren.com/{{pageId}}" target="_blank">{{pageName}}</a> 的管理员
		</div>
		<div class="detail"></div>
		<div class="btns">
			<button click="page_accept:{{uuid}},{{pageName}},{{pageId}},{{nid}}">接受</button>
			<button click="page_refuse:{{uuid}},{{nid}}" class="gray-btn">拒绝</button>
		</div>
		<div class="btn-x">
			<a class="x-to-delete" click="page_xxx:{{uuid}},{{nid}}"></a>
		</div>		
	</li>
{{/LI_SEC}}
