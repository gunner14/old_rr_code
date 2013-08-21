<html xmlns="http://www.w3.org/1999/xhtml">
<head>
{{>friend_header.tpl}}
</head>

<body id="friendpage">
<div id="container">

<body id="friendpage">
<script>
XN.getFileVersion(
[
'{{url_small_static}}/csspro/module/school-selector.css?ver=$revxxx$',
'{{url_small_static}}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$',
'{{url_small_static}}/js/provinceCity.js?ver=$revxxx$',
'{{url_small_static}}/js/xn.ymd.js?ver=$revxxx$'
]);
</script>

<div id="container">
  <div id="modifyGroupList" class="ModifyGroup-list">
		<ul></ul>
	</div>
  <iframe id="hashIframe" name="hashIframe" style="display:none;" src="{{domain_friend}}/ajaxProxy.html"></iframe>
  {{>header.tpl}}
  {{>sidebar.tpl}}
  <div id="content">
    <div class="page-title">
			<h2>好友</h2>
		</div>
    <div id="friendsMain" class="friends-main clearfix">
      <div id="sideBar" class="friends-sidebar">
        <div id="friendSearch" class="clearfix">
          <div id="search-input">
						<input type="text" class="input-text" name="filterFriends" id="friendSearchInput" maxlength="100" size="25" autocomplete="off" />
					</div>
        </div> {{! end friendSearch}}
        <ul class="CategoryList">
          <li><a href="javascript:;"><span class="i-all"></span>全部好友</a></li>
          <li><a href="javascript:;"><span class="i-seek"></span>寻找好友</a></li>
          <li><a href="javascript:;"><span class="i-special"></span>特别好友</a></li>
          <li><a href="javascript:;"><span class="i-new"></span>新添加的好友</a></li>
          <li class="page_item"><a href="javascript:;"><span class="i-pages"></span>公共主页</a></li>
          <li class="address_item">
            <a href="javascript:;"><span class="i-wab"></span>通讯录</a>
            <span class="sub" style="display:none;">
              <a title="msn">msn联系人</a>
              <a title="email">邮箱联系人</a>
            </span>
          </li>
        </ul>
        <h1>好友分组</h1>
        <ul id="groupList" class="GroupList">
          {{#friend_groups}}
            <li title="{{group_name}}"><a href="javascript:;"><span></span>{{group_name}}</a></li>
          {{/friend_groups}}
          <li title="未分组好友"><a href="javascript:;"><span class="NotGroup"></span>未分组好友</a></li>
        </ul>
        <div class="CreateGroup"><a href="javascript:;" class="createGroup">创建分组</a></div>
        <div class="InvitationFeiends">
          <a href="{{url_invite}}/Invite.do">邀请好友</a>
        </div>
        <div class="downloadclient">
          <a href="http://im.renren.com/desktop/rrsetup-2.0-10.exe"><img src="{{url_small_static}}/imgpro/button/downloadClient2.png" /></a>
        </div>
      </div> {{! end sideBar}}
      <div id="viewFriendsPanel" class="friends-listbox item_panel">
        <div class="fillet-tips FriendsGroup">
          <div class="j1"></div>
          <div class="j2"></div>
          <div class="fillet-content clearfix">
            <div class="ManagementGroup totalView" style="display:none;">
              <form>
                <dl>
                  <dt>筛选查找好友 :</dt>
                  <dd><select class="select" name="univ"><option selected="selected" value="">请选择</option></select> <select class="select" name="univ"><option selected="selected" value="">选择大学</option></select></dd>
                </dl>
              </form>
            </div>
 
            <div class="ManagementGroup float-none specialView" style="display:none;">
              <ul>
                <li><a href="javascript:;" class="editSpecialFriends">添加/移出特别好友</a></li>
                <li style="float: right;"><a href="{{url_main}}/profile.do?id={{host_id}}">查看特别好友效果</a></li>
              </ul>
            </div>

            <div class="ManagementGroup groupView" style="display:none;">
              <ul>
                <li><a href="javascript:;" class="createGroup">创建分组</a></li>
                <li><a href="javascript:;" class="editGroup">编辑分组</a></li>
                <li><a href="javascript:;" class="deleteGroup">删除分组</a></li>
              </ul>
            </div>

            <div id="pageTopPanel" class="pager-top float-right">
              <ol id="pagetop" class="pagerpro"></ol>
            </div>
          </div>
          <div class="j3"></div>
          <div class="j4"></div>
        </div> {{! end fillet-tips FriendsGroup}}
       
        <div id="userList" class="UserList">
          <ul></ul>
        </div>

        <div class="specialView friend-banner" style="display:none;"><img src="{{url_static}}/imgpro/friend/friend-banner-{{siteNameAbbr}}.png" /></div>
        <div class="fillet-tips FriendsGroup" style="display:none;">
          <div class="j1"></div>
          <div class="j2"></div>
          <div class="fillet-content clearfix">
            <div id="pageBottomPanel" class="pager-bottom float-right">
              <ol id="pagebottom" class="pagerpro"></ol>
            </div>
          </div>
          <div class="j3"></div>
          <div class="j4"></div>
        </div>       
      </div> {{! end viewFriendsPanel}}
      <div id="searchFriendsPanel" class="friends-listbox item_panel" style="display:none;"></div>
      <div id="pagesPanel" class="friends-listbox item_panel" style="display: none;height:3000px;">
        <iframe width="100%" height="100%" frameborder="0" marginwidth="0" marginheight="0" vspace="0" hspace="0" allowtransparency="true" scrolling="no"></iframe>
      </div>     
      
      <div id="adressList" class="wab-box item_panel clearfix" style="display:none;">
        <div class="wab-tab clearfix">
          <div class="wab-search">
            <input type="text" class="textbox" id="filterCard" autoComplete="off" />
            <input type="submit" value="搜索" class="searchbtn" />
          </div>
          <div class="wab-btn">
            <a id="exchangeCard" class="gray-btn" href="javascript:;"><span class="change-btn-icon">交换名片</span></a>
            <a id="showMyCard" class="gray-btn" href="javascript:;"><span class="card-btn-icon">我的名片</span></a>
            <a id="importContact" class="gray-btn" href="javascript:;"><span class="import-btn-icon">导入联系人</span></a>
          </div>
        </div>
        <div class="wab-content clearfix">
          <div id="wabList" class="wab-list clearfix">
            <h3 style="text-align:center; margin-top:10px;" ><img src="http://a.xnimg.cn/imgpro/bg/loading.gif" /></h3>
          </div>
          <div id="cardDetials" class="wab-detail"></div>
        </div>
      </div>
      <div class="Widget">
        {{>vertical-ad-ugc.tpl}}
      </div>
    </div> {{! end friendsMain}}
  </div> {{! end content}}
  {{>friend_footer.tpl}}
</div>  {{! end container}}
</body>
</html>
