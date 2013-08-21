<#if isNewsFeed == "false">
	<#if userID[0] == fID[0]>
		<a href="#nogo" onclick="javascript:readThisFeed('${id[0]}','${userID[0]}','minifeed');" class="delete">删除</a>
	</#if>
<#else>
	<a href="#nogo" class="delete" onclick="readHomeFeed('${id[0]}')">删除</a>
</#if>
