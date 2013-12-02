{{#LI_SEC}}
  <li class="request">
    <div class="figure">
      <a href="http://www.renren.com/profile.do?id={{from_id}}" target='_blank'>
         <img height="50" width="50" src="{{head_url}}"/>
      </a>
    </div>
    <div class="detail">
		<h4 class="header">
			<a href="http://www.renren.com/profile.do?id={{from_id}}" target='_blank'>{{from_name}}</a>请求与你建立{{loveTypeString}}关系
		</h4>
		<div class="section" style="display:none">
			<input id="feed_{{nid}}" type="checkbox" name="dispatchFeed" checked="checked" /> 发送新鲜事
		</div>
    </div>
    <div class="btns">
      <button click="showLove_accept:{{lifeEventId}},{{loveInfoId}},{{nid}}">接受</button>
      <button click="showLove_refuse:{{lifeEventId}},{{loveInfoId}},{{nid}}" class="gray-btn">忽略</button>
    </div>
    <div class="btn-x"><a class="x-to-delete" click="showLove_xxx:{{lifeEventId}},{{loveInfoId}},{{nid}}"></a></div>
  </li>
{{/LI_SEC}}
