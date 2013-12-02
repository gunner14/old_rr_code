{{#LI_SEC}}
  <li data-tagger_name="{{taggerName}}" data-tagger_id="{{taggerId}}" data-owner_id="{{ownerId}}" data-photo_id="{{photoId}}" data-leftToPhoto="{{leftToPhoto}}" data-photoWidth="{{photoWidth}}" data-topToPhoto="{{topToPhoto}}" data-frameHeight="{{frameHeight}}" data-frameWidth="{{frameWidth}}" class="request tagPhoto">
    <div class="figure">
      <a class="photoicon" title="浏览照片" target="_blank" href="http://photo.renren.com/photo/{{ownerId}}/photo-{{photoId}}">
          <img src="{{main_url}}">
      </a>
      <span id="picBox" class="frameOut">
        <span class="frameIn"></span>
      </span>
      <span id="picName" style="width:auto;height:auto;" class="tag">{{targetName}}</span>
    </div>
    <div class="detail">
      <h4 class="header">
        <a href="http://www.renren.com/profile.do?id={{taggerId}}">{{taggerName}}</a> 
        <span>在</span>
        <a href="http://www.renren.com/profile.do?id={{ownerId}}">{{ownerName}}</a> 
        <span>的相册里圈出了</span>
        {{#LINK}}
        <a href="http://www.renren.com/profile.do?id={{targetId}}">{{targetName}}</a>
        {{/LINK}}
        {{#NO_LINK}}
        {{targetName}}
        {{/NO_LINK}}
      </h4>
    </div>
    <div class="btns">
      <button click="tagPhoto_accept:{{id}},{{ownerId}},{{targetId}},{{nid}}">接受</button>
      <button class="gray-btn" click="tagPhoto_refuse:{{id}},{{ownerId}},{{targetId}},{{nid}}">拒绝</button>
    </div>
    <div class="btn-x"><a click="tagPhoto_xxx:{{id}},{{ownerId}},{{targetId}},{{nid}}" class="x-to-delete"></a></div>
  </li>
{{/LI_SEC}}
