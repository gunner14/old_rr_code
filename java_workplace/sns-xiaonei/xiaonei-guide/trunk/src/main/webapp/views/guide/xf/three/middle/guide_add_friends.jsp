<%@ page pageEncoding="utf-8" %>
<div id="guide-add-friends" class="add-friends" style="display:none">
	<h3 id="commendFriendMore">添加好友</h3>
	<h3 style="display:none;font-size:12px" id="commendFriendsShow" onclick="showAddFriend();"><a href="javascript:;">展开添加更多好友</a></h3>
	<span class="perfect">
		<a href="${applicationScope.urlWww}/profile.do?id=${requestScope.visiter.id}&amp;v=info&amp;act=edit">完善资料找到更多好友 &raquo;</a>
	</span>
    <div class="sg_selector clear-both" id="commendFriendsSelector">
    	<ul id="commendFriendsList" class="clearfix"></ul>
    </div>
    <div class="btns" id="commendFriendsBtns">
    	<input type="button" value="加为好友" class="input-submit" onclick="sendRequest();"/>
		<a href="javascript:void(0);" onclick="waitAfterAdd();">以后再说</a>
	</div>	
</div>
