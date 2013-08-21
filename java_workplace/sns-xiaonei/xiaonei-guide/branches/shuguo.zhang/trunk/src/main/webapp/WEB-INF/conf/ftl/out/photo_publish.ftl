<#assign num = 0/>
<#list count as c>
	<#assign num=num+c?number>
</#list>
<#if num == 1>
<#include "1,2">
<#elseif (num > 1)>
<#include "1,3">
</#if>
