<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<c:if test="${!empty olympic_buble}">
<style type="text/css">
.olympic-pop{display:block;position:absolute;left:220px;top:32px;left:433px;z-index:99999;}
.olympic-pop .olympic-pop-arrow{position:absolute;width:11px;height:7px;left:32px;top:-6px;background:url('http://a.xnimg.cn/imgpro/homeAd/olympic-arrow1.png') no-repeat;}
.olympic-pop .olympic-content-main{position:relative;width:284px;height:244px;width:310px;height:275px;background:none repeat scroll 0 0 #ffffff;border:1px solid #999999;border-radius:5px;}
.olympic-content-main .olympic-title{padding:15px;}

.olympic-content-main .olympic-detail{padding:0 15px 15px 15px;}
.olympic-detail .detail-avatar{float:left;}
.olympic-detail .olympic-word{float:left;margin-left:10px;width:158px;}
.olympic-detail .olympic-word .olympic-word-title{margin-top:5px;}
.olympic-detail .olympic-word .olympic-word-content{margin-top:10px;}

.olympic-content-main .olympic-medal{padding-bottom:15px;margin:0 15px 0 15px;border-bottom:1px solid #d9dce2;width:280px;}
.olympic-medal span{margin-left:15px;}
.olympic-medal a{color:#cc6633;margin-left:6px;cursor:pointer;width:21px;overflow:hidden;display:inline-block;line-height:12px;font-weight:bold;}

.olympic-content-main .olympic-hotnews{margin-top:15px;padding:0 15px 0 15px;}
.olympic-hotnews input{margin-left:157px;margin-right:5px;}
</style>
<c:set var="item" value="${olympic_buble}"></c:set>
<c:set var="metals" value="${OLYMPIC_2012_medals_toJson}"></c:set>
<c:set var="jumpurl" value="${item[\"olympic_bubble_jumpurl\"]}"/>
<c:set var="pic" value="${item[\"olympic_bubble_picurl\"]}"/>
<c:set var="title" value="${item[\"olympic_bubble_title\"]}"/>

<div class="olympic-pop" id="olympic-pop">
	<div class="olympic-content-main">
		<div class="olympic-title clearfix"><h2>人人看奥运 奥运看人人</h2></div>
		<div class="olympic-detail clearfix">
				<a class="detail-avatar" onclick="this.parentNode.parentNode.parentNode.style.display='none';" href="${jumpurl}" target="_blank"><img src="${pic}" width="110px" height="80px"></a>
			<div class="olympic-word">
					<a href="${jumpurl}" target="_blank" onclick="this.parentNode.parentNode.parentNode.parentNode.style.display='none';" class="olympic-word-title">${title}</a>
					<p class="olympic-word-content">${item["olympic_bubble_describe"]}
					<a href="${jumpurl}" target="_blank" onclick="this.parentNode.parentNode.parentNode.parentNode.style.display='none';" class="olympic-word-more" >[更多]</a>	
					</p>
			</div>
		</div>
		<div class="olympic-medal">
			<p>中国奖牌榜：
				<c:forEach items="${metals}" var="t" varStatus="abc">
					<c:if test="${abc.count>3}">
					<span>${t.key}<a href="${jumpurl}" target="_blank">${t.value}</a></span><!--金：1 银：2铜：3-->
					</c:if>
				</c:forEach>
			</p>
			<p>		奥运金牌榜：
				<c:forEach items="${metals}" var="t" varStatus="abc">
				<c:if test="${abc.count<=3}">
					<span>${t.key}<a href="${jumpurl}" target="_blank">${t.value}</a></span>	
				</c:if>
				</c:forEach>
			</p>
		</div>
		<div class="olympic-hotnews">
				<p><a href="${jumpurl}"  target="_blank" onclick="this.parentNode.parentNode.parentNode.parentNode.style.display='none';">${item["olympic_bubble_linkdoc"]}</a></p>
			<p style="margin-bottom:5px;"><a href="${jumpurl}" target="_blank" onclick="this.parentNode.parentNode.parentNode.parentNode.style.display='none';"> ${item["olympic_bubble_guessdoc"]}</a></p>
			<input type="checkbox" id ="olympic-no-notice"/><span>奥运期间不再提醒</span>
		</div>
		<a href="#nogo" onclick="this.parentNode.parentNode.style.display='none';return false;" class="close-button-alpha" style="position:absolute;top:12px;right:9px;"></a>
	</div>
	<span class="olympic-pop-arrow"></span>
</div>
<script type="text/javascript">
		XN.dom.ready(function(){
			var ele = document.getElementById('olympic-no-notice');
			XN.event.addEvent(ele,'click',function(){
				var value = ele.checked;
					new XN.net.xmlhttp({
						url:'http://www.renren.com/olympic/olympic_reminder',
						method:'post',
						data:'olympic_reminder='+value,
						onSuccess:function(){},
					});		
			});	
		});
</script>
</c:if>
