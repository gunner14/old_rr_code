{{#LI_SEC}}
	<li class="request">
		<div class="figure">
			<a href="http://www.renren.com/profile.do?id={{hostId}}" target="_blank">
				<img height="50" width="50" src="{{hostTinyHeadSource}}">
			</a>
		</div>
		<h4 class="header">
			<a href="http://www.renren.com/profile.do?id={{hostId}}" target="_blank">{{hostName}}</a> 创建了人人小站 <a href="http://zhan.renren.com/zhan/{{siteId}}?from=req" target="_blank">{{siteName}}</a>
		</h4>
		<div class="detail">
		</div>
		<div class="btns">
			<button click="xzInvite_accept:{{siteUrl}},{{hostId}},{{siteId}},{{siteName}},{{nid}}">关注</button>
			<button click="xzInvite_refuse:{{siteUrl}},{{hostId}},{{siteId}},{{nid}}" class="gray-btn">忽略</button>
		</div>
		<div class="btn-x">
			<a class="x-to-delete" click="xzInvite_xxx:{{siteUrl}},{{hostId}},{{siteId}},{{nid}}"></a>
		</div>		
	</li>
{{/LI_SEC}}
