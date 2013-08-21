<%@ page pageEncoding="utf-8" %>
<div id="newUserGuide">
	<div class="guidepanel">
    	<h2>欢迎你来到${domainIdentified["siteName"]}网 ${host.name}！</h2>
       	<div class="push-frd">
           	<h3>好友推荐</h3>
           	<div class="content">
               	<div class="frd-channel"> 
               		<a id="onlyCity" href="javascript:;" class="current">同乡好友</a> <a id="onlyOnline" href="javascript:;">在线好友</a> 
               		<a id="onlyBoy" href="javascript:;">只看帅哥</a> <a id="onlyGirl" href="javascript:;">只看美女</a> 
               	</div>
               	<div class="choose-frd">
                 	<p id="typeTitle" class="title"> 同乡好友
                   		<select id="province"><option value="">选择省份</option></select>
                   		<select id="city"><option value="">选择城市</option></select>
                 	</p>
                 	<div id="personView">
                   		<ul class="figure"><li class="loading">loading...</li></ul>
                 	</div>
                 	<div class="showmore-frd"> <a href="javascript:;" id="leftMore"></a> 更多好友 <a href="javascript:;" id="rightMore"></a> </div>
               	</div>
             </div>
		</div>                
       <div class="search-frd">
          	<h3>搜索好友</h3>
            <div class="search-form">
            	<div class="search-channel"> 
            		<a id="byHome" href="javascript:;" class="current">找老乡</a>
            		<a id="byName" href="javascript:;">按用户名查找</a> 
            	</div>
            	<div id="byHomeView" class="bycity gform">
            		<form id="searchSeniorForm" action="${applicationScope.urlBrowse}/searchEx.do?actionid=600301&amp;ref=nb_search_sametown" method="post" target="_blank">                   
               			<span>家乡: </span>
               			<select name="homeProvince" id="homeProvince" onchange="getCityByChangeProvinceid(this.selectedIndex,'homeCity')">
               				<option>选择省份</option>
              			</select>
               			<select name="homeCity" id="homeCity" onblur="changeCodeCity('homeCity',this.selectedIndex)">
							<option value="">城市</option>
						</select>
                		<span>&nbsp;&nbsp;性别 : </span>
                		<select name="gender">
                			<option value="">不限</option>	
                			<option value="男生">男</option>	
                			<option value="女生">女</option>
                		</select>
                		<span>&nbsp;&nbsp;年龄 : </span>
                		<select id="selAge" name="selAge"><option value="">不限</option></select>
                   		<input type="submit" class="input-submit searchex" value="搜索" />                        
						<input type="hidden" name="p" id="p" />
						<input type="hidden" name="q" value="" />
						<input type="hidden" name="s" value="0" />
                	</form>
              	</div>                   
              	<div id="byNameView" class="byname gform" style="display:none">
              		<form id="searchByNameForm" action="${applicationScope.urlBrowse}/searchEx.do?actionid=600303&amp;ref=nb_search_name" method="post" target="_blank">
                		<input id="inputname" type="text" class="input-text" name="q" value="输入用户姓名进行查找..." />
                		<input type="hidden" name="s" value="0" />                      
                		<input type="submit" class="input-submit search" value="搜索" />
               		</form>
              	</div>
            </div>
    	</div>
	</div>
</div>
<div id="replyDiv" class="reply-notify news-feed" style="display:none"><%-- 异步加载 --%>
	<div class="section-header">
		<h2>新留言及回复<span class="stat"><a id="gossipReplyCount" href="${applicationScope.urlWww}/myreplylist.do"></a></span></h2>
		<span class="more" id="replyMoreDiv"><a href="${applicationScope.urlWww}/myreplylist.do">更多</a></span>
	</div>
	<div id="opilist" class="feed-story"></div>
</div>	
<%@ include file="/pages/register/guide_news_feed.jsp"%><%--新鲜事 --%>
