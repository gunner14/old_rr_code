{{#LI_SEC}}
<li class="request">
	<div class="figure">
		<a href="{{app_url}}" target="_blank">
			<img height="75" width="75" src="{{app_head_url}}">
		</a>
	</div>
	<h4 class="header"><a href="{{app_url}}" target="_blank">{{app_name}}</a> 申请关联您的公共主页 <span><a href="{{page_url}}" target="_blank">{{page_name}}</a>,关联后可添加应用相关模块。</span></h4>
	<div class="description">{{current_time}}</div>
	<div class="detail"></div>
	<div class="btns">
		<button click="apppage_accept:{{acceptUrl}},{{app_id}},{{page_id}},{{signature}},{{receiverId}},{{notify_id}},{{page_setting_url}},{{nid}}">接受</button>
		<button click="apppage_refuse:{{refuseUrl}},{{app_id}},{{page_id}},{{signature}},{{receiverId}},{{notify_id}},{{nid}}" class="gray-btn">忽略</button>
	</div>
	<div class="btn-x">
		<a class="x-to-delete" click="apppage_xxx:{{refuseUrl}},{{app_id}},{{page_id}},{{signature}},{{receiverId}},{{notify_id}},{{nid}}"></a>
	</div>		
</li>
{{/LI_SEC}}
