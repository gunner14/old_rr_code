{{#LI_SEC}}
  <li class="request">
    <div class="figure">
      <a href="http://www.renren.com/profile.do?id={{id}}" target='_blank'>
         <img height="50" width="50" src="{{head}}"/>
      </a>
    </div>
    <h4 class="header">
      <a href="http://www.renren.com/profile.do?id={{id}}" target='_blank'>{{name}}</a>
      <span class="gray-text">{{#test}}{{T}}{{/test}}
      {{#NO_LINK}}({{net}}){{/NO_LINK}}{{#LINK}}{{net}}{{/LINK}}
</span>
    </h4>
    <div class="btns">
      <button click="friend_accept:{{id}},{{name}},{{nid}}">接受</button>
      <button click="friend_refuse:{{id}},{{name}},{{nid}}" class="gray-btn">拒绝</button>
    </div>
    <div class="detail">
      <div class="section"><q>{{why}}</q></div>            
      <div class="section description" id="description_{{id}}">
      </div>
    </div>
    <div class="btn-x"><a class="x-to-delete" click="friend_xxx:{{id}},{{nid}}"></a></div>
  </li>
{{/LI_SEC}}
