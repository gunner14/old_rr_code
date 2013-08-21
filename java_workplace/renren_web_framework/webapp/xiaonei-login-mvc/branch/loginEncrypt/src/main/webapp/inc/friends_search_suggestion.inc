<%@ page pageEncoding="UTF-8" %>
<div class="search_container" style="left:0px;top:0px;">
	<div id="search_f1_friends_search" class="search_com" style="display:none;width:102px;">
		<div id="f2_container_friends_search" class="com_container"></div>
	</div>
	<div id="friendPhoto_friend_search" style="top:-10px;left:220px;"></div>
 </div>
<script>

function load_friends_friend()
{
	$('fid_in_sidebar').value='friend_search';
	if( (typeof friendLists == 'undefined') == true)
	{
		var script = document.createElement("script");
		script.src='${applicationScope.urlWww}/friendlistforgift.do';
		document.getElementsByTagName("head")[0].appendChild(script);
	}
	var fri_2 = new _autoSearch_sidebar($('query_name'),$('f2_container_friends_search'),$('friendPhoto_friend_search'),'');
}
</script>