{{#LI_SEC}}
  <li class="request">
    <div class="figure">
      <a href="http://zhan.renren.com/{{siteUrl}}?from=req" target="_blank">
        <img height="80" width="80" src="http://hdn.xnimg.cn/photos/{{siteTinyHeadSource}}">
      </a>
    </div>
    <h4 class="header">
      <a href="http://www.renren.com/profile.do?id={{hostId}}" target="_blank">{{hostName}}</a> 推荐你关注小站 <a href="http://zhan.renren.com/{{siteUrl}}?from=req" target="_blank">{{siteName}}</a>
    </h4>

    <div class="detail">
        <div class="sitedescription">{{siteDescription}}</div>
        <div class="commend">
            <span><a class="wh30" href="http://www.renren.com/profile.do?id={{hostId}}" target="_blank"><img title="{{hostName}}" src="{{hostTinyHeadSource}}"></a></span><div class="reason">{{content}}</div>
        </div>
    </div>

    <div class="btns">
      <button click="xiaozhan_accept:{{siteUrl}},{{hostId}},{{siteId}},{{nid}}">接受</button>
      <button click="xiaozhan_refuse:{{siteUrl}},{{hostId}},{{siteId}},{{nid}}" class="gray-btn">拒绝</button>
    </div>
    <div class="btn-x">
            <a class="x-to-delete" click="xiaozhan_xxx:{{siteUrl}},{{hostId}},{{siteId}},{{nid}}"></a>
    </div>

  </li>
{{/LI_SEC}}
