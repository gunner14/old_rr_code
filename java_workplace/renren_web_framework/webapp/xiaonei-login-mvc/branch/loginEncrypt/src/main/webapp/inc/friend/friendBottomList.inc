<script type="text/javascript">
var commonTimer = 0;
function tryGetCommonFriend(count){
	ajaxGetCommonFriend(count);
	commonTimer = setTimeout('ajaxGetCommonFriend(\''+count+'\')',1000);
}
function ajaxGetCommonFriend(count){
	var postUrl = "/ajxgetcmn.do";
	var pars = "count=" + count + "&type = 3";
	var myAjax = new Ajax.Request(postUrl, {
		method: 'get',
		parameters: pars,
		onComplete: commonFriendRet,
		onFailure:  commonFriendErr
		}
	);	
} 
function commonFriendErr(){
}
function commonFriendRet(r){
	var	commonFriendData;
	eval("commonFriendData="+r.responseText);
	var friendCommonList = commonFriendData.cfriendList;
	if (friendCommonList == null || friendCommonList.length<1){
		return;
	}
	clearTimeout(commonTimer);
	var content = "	<h3 class=\"pyourmknow\">你可能认识的人</h3>"
					+ "<div class=\"boxcont\"><table class=\"personIconList\"><tr>";
	var i=0;
	for(i=0;i<friendCommonList.length;i++)
	{
		if(i%8==0 && i != 0){
			content += "</tr><tr>";
		}
		content +=	 "<td class=\"personInfo\"><div class=\"personIcon\"><table><tr><td><a href=\"${applicationScope.urlProfile}/profile.do?id="
							+ friendCommonList[i].id + "\""
							+ "target=\"_blank\"><img src=\""
							+ friendCommonList[i].tiny_Url + "\""
							+ "alt = \""
							+ friendCommonList[i].name + "\" />"
							+ "</a></td></tr></table></div><div class=\"personName\"><a href=\"${applicationScope.urlProfile}/profile.do?id="
							+ friendCommonList[i].id + "\""
							+ "target=\"_blank\">"
							+ friendCommonList[i].name
							+ "</a></div></td>";
	};
	content = content + "</tr></table></div>";
	document.getElementById("friendBottomList1").innerHTML = content;
	document.getElementById("friendBottomListTitle").innerHTML = "";
}
function ajaxGetSearchFriend(count){
	var postUrl = "/ajax_get_common_friend.do";
	var pars = "count=" + count + "&type = 2";
	var myAjax = new Ajax.Request(postUrl, {
		method: 'post',
		parameters: pars,
		onComplete: SearchFriendRet,
		onFailure:  SearchFriendErr
		}
	);	
}
function SearchFriendErr(){
}
function SearchFriendRet(r){
	var	SearchFriendData;
	eval("SearchFriendData="+r.responseText);
	var friendSearchList = SearchFriendData.friendSearchList;
	var fromWord = SearchFriendData.fromWord;
	var urlSearch = SearchFriendData.urlSearch;
	var moreLinkNext = SearchFriendData.moreLinkNext;
	var content = "<h3><span class=\"more\"><a target=\"_blank\" href=\""
				+ urlSearch
				+ "/ns.do?"
				+ moreLinkNext + "\">查看更多</a></span>"
				+ fromWord + "</h3>"
				+ "<div class=\"boxcont\"><table class=\"personIconList\"><tr>";
	var i=0;
	for(i=0;i<friendSearchList.length;i++)
	{
		if(i%8==0 && i != 0){
			content += "</tr><tr>";
		}
		content += "<td class=\"personInfo\"><div class=\"personIcon\"><table><tr><td><a href=\"${applicationScope.urlProfile}/profile.do?id="
					+ friendSearchList[i].id + "\" target=\"_blank\"><img src=\""
					+ friendSearchList[i].tinyFullUrl + "\"alt=\""
					+ friendSearchList[i].name + "\" /></a></td></tr></table></div><div class=\"personName\"><a href=\"${applicationScope.urlProfile}/profile.do?id="
					+ friendSearchList[i].id + "\" target=\"_blank\">"
					+ friendSearchList[i].name + "</a></div></td>"
	}		
	content = content + "</tr></table></div>";
	document.getElementById("friendBottomList2").innerHTML = content;
}
</script>


<div id="friends" class="wbg_topborder">


	<h1 id="friendBottomListTitle">
		
	</h1>
	
	 
	<p class="moremarginbottom">
		
	</p>
	
	<div class="box" id = "friendBottomList1">
		<script>tryGetCommonFriend('16');</script>
	</div>	
	<!--
	<div class="box" id="friendBottomList2">
		<script>ajaxGetSearchFriend('8');</script>
	</div>
	-->
</div>


