<%@ page pageEncoding="utf-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://struts.apache.org/tags-tiles" prefix="tiles"%>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<style>
/*fools day by zhouming*/
.countdown{clear:both;text-align:left;padding:0 0 8px;}
.countdown  span{color:#888}
.ricky{padding: 2px 6px;position: absolute;top:0;right:0; background:#FFFBC1; border: 1px solid #F9BD6D; width: 200px;text-align:left;}
.ricky-arrow{ display: block; position: absolute; z-index: 100; left: 32px; margin-top: -8px; width: 10px; height: 8px; overflow: hidden; background: url(${applicationScope.urlStatic}/imgpro/bg/point-bg.png) no-repeat -20px top;}
.prizes{margin:12px 0 5px;}
.prize-data{border:1px solid #DDD;border-collapse:collapse;background:#FFF;width:200px;margin-bottom:5px}
.prize-data caption{text-align:left;padding-bottom:3px;}
.prize-data th,.prize-data td{border:1px solid #DDD;text-align:center;border-collapse:collapse;padding:3px 5px}
.prize-data th{width:70px;text-align:left;font-weight:normal}
.prize-data p{color:#888;line-height:18px;}
.font-orange{color:#F60;font-weight:bold}
.clock-hot{vertical-align:text-bottom;}

.ricky-clock{vertical-align:middle;*margin-top:2px}
.ricky-clock:nth-of-type(1){margin-bottom:2px}

.grade-view{width:116px;padding:3px 0 0;}
.ricky-words{color:#666;text-align:right}
.lottery{text-align:left;border:1px solid #DDD;background:#F1F6F9;padding:8px 10px;margin:5px 0 10px}
.lottery-time{height:30px;line-height:30px;color:#888}
.lottery-btn{background: url(${applicationScope.urlStatic}/imgpro/button/lottery-btn.png) no-repeat;height:23px;width:85px;display:block;float:left;margin:5px 8px 0 0}
.fools-day-remind{margin:5px 0 20px;}
.fools-header{border-bottom:1px solid #E3EEF8;line-height:1.8em;margin-bottom:8px;text-align:right;color:#4181C4}
.fools-header h2 {float:left;}
.fools-header img{margin-right:3px;vertical-align:text-bottom}
.fools-day-remind span.fools-words{font-size:14px;color:#333;font-weight:normal}
.fools-body p{margin:0 0 5px 50px;}
.fools-title{float:left;}
.repeatedly{font-size:12px;font-weight:normal;margin-left:10px;}

/*point hints by zhouming@2010-3-16*/
.home ul.point-hint{margin-bottom:10px;}
.home ul.point-hint li a{display:inline-block;}
.home ul.point-hint li img{vertical-align:text-bottom;margin-right:3px}
</style>
<c:if test="${!empty yurenInfo}">
<script> 
function vipDraw(){
    currentTimeStamp.stop();
    new Ajax.Request("http://act."+XN.env.domain+"/yuren/op/draw", {
        method: "post",
        onSuccess: function(o){
            $('vip_draw').innerHTML = o.responseText;
            $('vip_draw').eval_inner_JS();
        },
        onFailure: function(){
            XN.DO.alert({
                title: "提示",
                message: "系统繁忙，请稍后再试"
            });
        }
    });
}
</script>
<div id="vip_draw" style="display:none">
</div>
<script>
<%--
window.vipYurenTips = function(show){
    if (show) {
        $('vipYurenTips').show();
    }
    else {
        $('vipYurenTips').hide();
    }
}
--%>
window.yurenRemain=${yurenInfo.remainDrawTime};
window.yurenNext=${yurenInfo.nextDrawTime.time};

(function(){
	var global=[];
	var timer={
		config:{
			first:600,
			seconed:0
		},
		init:function(container,time,p1,p2){
			var remainSeconds,
				_this=this;
			this.container=container;
			remainSeconds=Math.round(time/1000);
			this.print(remainSeconds);
			this.interval=setInterval(function(){
				remainSeconds-=1;
				_this.print(remainSeconds);
				if(remainSeconds==_this.config.first){
					p1 && p1();
					
				}else if(remainSeconds==_this.config.seconed){
					clearInterval(_this.interval);
					p2 && p2();
					return;
				}
			},1000)
			currentDate=null;
		},
		setContainer:function(c){
			this.container=c;
		},
		print:function(num){
			var minutes=Math.floor(num/60)
				,seconds=num%60;
			this.container.innerHTML=this.fill(minutes)+"分"+this.fill(seconds)+"秒";
		},
		fill:function(num){
			return num<10?"0"+num:num;
		}
	}
	window.currentTimeStamp=function(container,time,p1,p2,c){
		var temp=function(){}
			,obj;
		temp.prototype=timer;
		obj=new temp();
		obj.config={};
		for(var i in timer.config){
			obj.config[i]=timer.config[i];
		}
		for(var n in c){
			obj.config[n]=c[n];
		}
		obj.init(container,time,p1,p2);
		global.push(obj.interval);
		obj=null;
	}
	currentTimeStamp.stop=function(){
		for(var i=0,n=global.length;i<n;i++){
			clearInterval(global[i]);
		}
	}
})();
<c:if test="${!yurenInfo.canDraw}">
XN.DOM.ready(function(){
	currentTimeStamp($("vipCountDown"),yurenRemain,vipYurenHot,vipYurenCDDone,{
		first:600,
		seconed:0
	})
	if(yurenRemain<=600000){
		vipYurenHot();
	}
});
</c:if>
<c:if test="${yurenInfo.canDraw}">
XN.DOM.ready(function(){
	currentTimeStamp($("vipCountDown"),yurenRemain,function(){},function(){$('vipCountDown').innerHTML=""},{
		first:600,
		seconed:0
	})
});
</c:if>
window.vipYurenHot=function(){
	var yurenImg=$("vipYurenImg");
	yurenImg.src="${applicationScope.urlSmallStatic }/imgpro/icons/clock-hot.png";
	yurenImg.className="clock-hot";
	yurenImg.width=16;
	yurenImg.height=19;
}
window.vipYurenCDDone = function(){
    var yurenLottery = $("yurenLottery");
    yurenLottery.className = "lottery clearfix";
    yurenLottery.innerHTML = "<p>抽取整蛊大战奖品，<a href=\"http://act.${domainIdentified['sys.domain']}/yuren/home#xiangxishuoming\">了解更多</a></p>" +
    "<p class=\"lottery-time\"><a href=\"#nogo\" onclick=\"vipDraw()\" class=\"lottery-btn\"></a>下次：<span  id=\"vipCountDown\">&nbsp;</span></p>"
    currentTimeStamp($("vipCountDown"), 3600000, vipYurenHot, vipYurenCDDone, {
        first: 600,
        seconed: 0
    })
}
window.vipYurenDrawDone=function(){
	var yurenLottery=$("yurenLottery");
	yurenLottery.className="countdown";
	yurenLottery.innerHTML='<img id="vipYurenImg" class="ricky-clock" src="${applicationScope.urlSmallStatic }/imgpro/icons/ricky-clock.png" alt="" width="11" height="11"/>'+
		'&nbsp;<a href="http://act.${domainIdentified['sys.domain']}/yuren/home">整蛊大战</a> 抽奖倒计时：<span  id="vipCountDown">&nbsp;</span>';
	currentTimeStamp($("vipCountDown"),vipGetTimeStamp(new Date().getTime()),vipYurenHot,vipYurenCDDone,{
		first:600,
		seconed:0
	})
}
window.vipGetTimeStamp=function(time){
	var currentDate=new Date()
		,days
		,hours
		,remain;
	currentDate.setTime(time);
	days=currentDate.getDate();
	hours=currentDate.getHours()+1;
	if(hours==24){
		hours=0;
		days+=1;
	}
	remain={
		setFullYear:currentDate.getFullYear(),
		setMonth:currentDate.getMonth(),
		setDate:days,
		setHours:hours,
		setMinutes:0,
		setSeconds:0
	}
	var date=new Date();
	for(var i in remain){
		date[i](remain[i]);
	}
	return date.getTime()-time+1000;
}
</script>
<c:if test="${yurenInfo.canDraw}">
	<div id="yurenLottery" class="lottery clearfix">
		<p>抽取整蛊大战奖品，<a href="http://act.${domainIdentified['sys.domain']}/yuren/home#xiangxishuoming">了解更多</a></p>
		<p class="lottery-time"><a href="javascript:;" onclick="vipDraw()" class="lottery-btn"></a>下次：<span  id="vipCountDown">&nbsp;</span></p>				
	</div>
</c:if>
<c:if test="${!yurenInfo.canDraw}">
	<div id="yurenLottery" class="countdown">
		<img id="vipYurenImg" class="ricky-clock" src="${applicationScope.urlSmallStatic }/imgpro/icons/ricky-clock.png" alt="" width="11" height="11"/>&nbsp;
		<a href="http://act.${domainIdentified['sys.domain']}/yuren/home">整蛊大战</a> 抽奖倒计时：<span  id="vipCountDown">&nbsp;</span>
	</div>
</c:if>
<%--<div style="position: relative; z-index: 10; display: none" class="" id="vipYurenTips">
	<div class="ricky">
	<span class="ricky-arrow"></span>
	<div id="" class="grade-view">
		<span class="grade-num">${myLevel}级</span>
		<span class="grade-cur">${currentScore}/${nextLevelScore}</span>
		<div class="slide-bar">
			<span style="width: ${currentScore*100/nextLevelScore}%;" class="slide-bar-cur"></span>
		</div>
	</div>				
	<div id="" class="prizes">
		<table class="prize-data">
			<caption>橙色用户抽奖特权：</caption>
			<tr>
				<th></th>
				<td class="font-orange">橙名</td>
				<td>普通</td>
			</tr>
			<tr>
				<th>礼券</th>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt=""  width="12" height="9"/></td>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt="" width="12" height="9"/></td>
			</tr>
			<tr>
				<th>普通道具</th>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt="" width="12" height="9"/></td>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt="" width="12" height="9"/></td>
			</tr>
			<tr>
				<th>高级道具</th>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt="" width="12" height="9"/></td>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/delete.gif" alt=""  width="9" height="9" /></td>
			</tr>
			<tr>
				<th>成为VIP身份</th>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt="" width="12" height="9"/></td>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/delete.gif" alt="" width="9" height="9" /></td>
			</tr>
			<tr>
				<th>实物奖品：
				<p>Nokia N97<br />iPhone 3GS<br />手机充值卡<br />Polo轿车</p></th>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/choose.gif" alt="" width="12" height="9"/></td>
				<td><img src="${applicationScope.urlSmallStatic }/imgpro/bg/delete.gif" alt=""  width="9" height="9" /></td>
			</tr>
		</table>
		<p class="ricky-words">*连续登录7天，成为<span class="lively-user">橙名用户</span></p>
	</div>
	</div>
</div>--%>
</c:if>
