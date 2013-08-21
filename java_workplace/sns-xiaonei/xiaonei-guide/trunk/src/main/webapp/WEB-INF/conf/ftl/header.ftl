<#assign vType = "newsfeed"/>
<#assign isNewsFeed = true/>
<#if visitType[0]=="minifeed">
	<#assign vType = "minifeed"/>
	<#assign isNewsFeed = false/>
</#if>
<#assign vType = vType+"&sfet="+f_type[0]+"&fin="+feedIndex[0]/>