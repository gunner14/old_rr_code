<%@ page pageEncoding="utf-8" %>
<%@page import="com.xiaonei.platform.core.cache.MemCacheManager"%>
<%@page import="com.xiaonei.platform.core.cache.MemCacheKeys"%>
<%
	MemCacheManager mcm = com.xiaonei.platform.core.cache.MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);  
	if(!"display".equals(mcm.get("guideTopNotice"+user.getId()))){			
%>
	<div id="home_top_notice" class="notice-holder" style="position:relative;border:0pt none;" >
	    <a style="position:absolute;left:521px;top:7px" onclick="if($('home_top_notice'))$('home_top_notice').hide();ajax_set_mem_with_expiry('guideTopNotice${visiter.id}','1');" href="javascript:;">关闭</a>
	    <a href="http://qifu.${applicationScope.domain}" onclick="if($('home_top_notice'))$('home_top_notice').hide();ajax_set_mem_with_expiry('guideTopNotice${visiter.id}','1');" style="display:block; background:url(${applicationScope.urlStatic}/imgpro/notice/10yunnan.png) no-repeat;height:130px;text-decoration:none" target="_blank">&nbsp</a>
	</div>	
<%
	}
%>