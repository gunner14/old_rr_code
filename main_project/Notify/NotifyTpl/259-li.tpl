{{#LI_SEC}}
  <li data-host_name="{{name}}" data-host_id="{{fromId}}" class="request">
    <div class="figure">
      <a href="http://www.renren.com/profile.do?id={{fromId}}" target="_blank"><img height="50" width="50" src="{{tiny_url}}"></a>
    </div>
    <div class="detail">
      <h4 class="header"><a href="http://www.renren.com/profile.do?id={{fromId}}" target="_blank">{{name}}</a></h4>
      <div class="section">
        {{name}}怕错失你的联系方式，在通讯录中请求和你交换名片<a href="javascript:void(0);" click="addr_showcard:{{fromId}}">TA的名片</a>
      </div>
      <div class="section description">
        <div class="cards-detail" style="display:none"></div>
        <p class="blue-tip">交换名片可以获取好友详细联系方式，并收到TA的资料更新</p>
        <a href="http://friend.renren.com/myfriendlistx.do#item_5" target="_blank">查看通讯录 »</a>
      </div>
    </div>
    <div class="btns">
      <button click="addr_accept:{{fromId}},{{nid}}">接受</button>
      <button class="gray-btn" click="addr_refuse:{{fromId}},{{nid}}">拒绝</button>
    </div>
    <div class="btn-x"><a click="addr_xxx:{{fromId}},{{nid}}" class="x-to-delete"></a></div>
  </li>
{{/LI_SEC}}

