{{#LI_SEC}}
  <li class="request" data-link="{{FORUM_LINK}}">
    <div class="figure">
      <a target="_blank" href="http://www.renren.com/profile.do?id={{FROM_ID}}">
        <img src="{{FROM_HEAD_URL}}">
      </a>
    </div>
    <div class="detail">
      <h4 class="header">
        <a target="_blank" href="http://www.renren.com/profile.do?id={{FROM_ID}}">{{FROM_NAME}}</a> 邀请你加入小组 <a target="_blank" href="{{FORUM_LINK}}">{{FORUM_NAME}}</a> 
      </h4>
      <div class="section description">
        小组介绍:{{FORUM_DESC}}
      </div>
    </div>
    <div class="btns">
      <button click="xiaozuInvite_accept:{{ACCEPT_LINK}},{{nid}}">接受</button>
      <button click="xiaozuInvite_refuse:{{REFUSE_LINK}},{{nid}}" class="gray-btn">忽略</button>
    </div>
    <div class="btn-x"><a class="x-to-delete" click="xiaozuInvite_xxx:{{REFUSE_LINK}},{{nid}}"></a></div>
  </li>
{{/LI_SEC}}
