${"\r"}
<#compress>
<#include "1,0">
<li id="feed${id[0]}">
        <#include "1,4">
        <h3><a href="http://renren.com/profile.do?id=${fID[0]}&ref=${vType}" target="_blank">${fName[0]}</a> 提问
        <#if url?exists>
            <a href="${url[0]}">${title[0]}</a>
            <#else>
            ${title[0]}
        </#if>
        </h3>
        <#if body?exists>
           <div class="content">
                <blockquote>
                    ${body[0]}
                </blockquote>
           </div>
        </#if>
        <div class="details">
                <div class="legend">
                    <img alt="问答" lala="http://app.xnimg.cn/application/20090923/14/55/L909342619676SJS.gif" src="http://s.xnimg.cn/a.gif" onload="feed_img_delay_load(this);" class="iStatus"/>
                    <span class="duration"><@format>${time[0]}</@format></span>
                </div>
        </div>
<#include "1,1">
</li>
</#compress>