{{#LI_SEC}}
  <li class="request">
    <div class="figure">
      <a href="http://www.renren.com/profile.do?id={{userId}}" target="_blank">
        <img class="" height="50" width="50" src="{{userHeadImag}}">
      </a>
    </div>
    <div class="section">
      <img src="http://a.xnimg.cn/imgpro/icons/invite-prd.png"> <a href="http://renren.com/profile.do?id={{userId}}" target="_blank">{{userName}}</a> 创建了公共主页 <a href="http://page.renren.com/{{pageId}}" target="_blank">{{pageName}}</a>
    </div>
    <div class="detail"></div>
    <div class="btns">
      <button click="pageInvite_accept:{{rid}},{{pageName}},{{pageId}},{{nid}}">关注</button>
      <button click="pageInvite_refuse:{{rid}},{{pageId}},{{nid}}" class="gray-btn">忽略</button>
    </div>
    <div class="btn-x">
      <a class="x-to-delete" click="pageInvite_xxx:{{rid}},{{pageId}},{{nid}}"></a>
    </div>
  </li>
{{/LI_SEC}}
