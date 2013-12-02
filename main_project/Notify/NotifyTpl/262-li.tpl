{{#LI_SEC}}
  <li class="request">
    <div class="figure">
      <a target="_blank" href="{{accept_url}}">
        <img height="75" width="75" src="{{app_logo}}">
      </a>
    </div>
    <div class="detail">
      <h4 class="header"><a target="_blank" href="http://www.renren.com/profile.do?id={{sender_id}}">{{sender_name}}</a> 在 <a target="_blank" href="{{accept_url}}">{{app_name}}</a> 中给你发送了一个邀请：</h4>
      <div class="section">{{user_msg}}</div>
      <div class="section description">{{current_time}}</div>
    </div>
    <div class="btns">
      <button click="appmessage_accept:{{request_id}},{{app_id}},{{app_type}},{{accept_url}},{{nid}}">接受</button>
      <button class="gray-btn" click="appmessage_refuse:{{request_id}},{{app_id}},{{app_type}},{{nid}}">忽略</button>
    </div>
    <div class="btn-x">
      <a click="appmessage_xxx:{{request_id}},{{app_id}},{{app_type}},{{nid}}" class="x-to-delete"></a>
    </div>
  </li>
{{/LI_SEC}}
