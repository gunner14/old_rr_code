{{#LI_SEC}}
  <li class="request">
    <div class="figure">
      <a href="http://www.renren.com/profile.do?id={{r_id}}" target="_blank">
        <img height="50" width="50" src="{{r_head}}">
      </a>
    </div>
    <div class="header">
      <h4>
        <a href="http://www.renren.com/profile.do?id={{r_id}}" target="_blank">{{r_name}}</a>
      </h4>
    </div>
    <div class="btns">
      <button click="tuijian_accept:{{r_id}},{{r_name}},{{r_head}},{{m_id}},{{nid}}" class="accept">加为好友</button>
      <button click="tuijian_refuse:{{r_id}},{{r_name}},{{m_id}},{{nid}}" class="gray-btn">忽略</button>
    </div>
    <div class="detail">
      <div class="section description">
        &mdash;&mdash; <a href="http://www.renren.com/profile.do?id={{m_id}}">{{m_name}}</a> 推荐
      </div>
    </div>
    <div class="btn-x"><a click="tuijian_xxx:{{r_id}},{{m_id}},{{nid}}" class="x-to-delete"></a></div>
  </li>  
{{/LI_SEC}}
