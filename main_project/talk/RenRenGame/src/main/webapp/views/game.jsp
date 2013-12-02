<!DOCTYPE html>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<%@ page isELIgnored="false" %> 
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<link href="css/layout.css" rel="stylesheet" type="text/css" media="all" />
<link href="css/game_index.css" rel="stylesheet" type="text/css" media="all" />
<title>游戏助手</title>
<script>window["_GOOG_TRANS_EXT_VER"] = "1";</script>
</head>
<body scroll="no">
<div class="wrapper">
	<div class="banner-and-recommend clearfix">
        <div class="appShow">
            <div id="appImg" class="img" style="background-image: url(http://gadget.talk.renren.com/images/top1poker20120802.jpg); ">
               <div class="appNum">
                   <a href="javascript:;" name="numLink" class="cur">1</a>
                   <a href="javascript:;" name="numLink" class="">2</a>
                   <a href="javascript:;" name="numLink" class="">3</a>
                   <a href="javascript:;" name="numLink" class="">4</a>
               </div>
            </div> 
            <div class="border-conners-left-top"></div>
             <div class="border-conners-left-bottom"></div>    
              <div class="border-conners-right-top"></div>    
               <div class="border-conners-right-bottom"></div>             
        </div>
        <div class="recommend">
            <div class="border-conners-left-top"></div>
             <div class="border-conners-left-bottom"></div>    
              <div class="border-conners-right-top"></div>    
               <div class="border-conners-right-bottom"></div> 
        	<a target="_blank" href="http://apps.renren.com/rxqiuqiu/?s=3&rrlobby_hot"><img src="./images/hotrxqq20120920.jpg"></a>
            <ul>
            	<li class="recommend_icon"><img src="./images/recommend_icon.png"><a target="_blank" href="http://apps.renren.com/rxqiuqiu/?s=3&rrlobby_hot">热血球球2</a></li>
                <li><p>《热血球球2》，新服火爆开启。还原真实赛场，大牌球星任你掌控！</p></li>
            </ul>            
        </div>
        <div class="games">
        	<div class="games-header clearfix">
            	<span class="games-header-title">您可能喜欢的社区游戏</span><a class="more" target="_blank" href="http://app.renren.com/?rrlobby_app">更多&gt;&gt;</a>
            </div>
            <div class="games-body clearfix">
            <c:forEach var="game" items="${gameSpreadList}" varStatus="status">
            	<div class="game <c:if test="${status.index % 3 == 2}">lastGame</c:if>">
                	<a class="game-cover"  target="_blank" href="${game.appLink}">
                    <img src="${game.appLogo}">
                  </a>
                  <span class="app-round-corner"></span>    
                  <ul>
                    <li class="game-name">
                      <a target="_blank" href="${game.appLink}">${game.appName}</a>
                    </li>
                    <li><p class = "game-tip">${game.userNum}人在玩</p></li>
                    <li><p class = "game-style">${game.cate}</p></li>
                  </ul>
              </div>
            </c:forEach>
            </div>
            <div class="border-conners-left-top"></div>
            <div class="border-conners-left-bottom"></div>    
            <div class="border-conners-right-top"></div>    
            <div class="border-conners-right-bottom"></div>        
        </div>      
        <div class="hotgames-per-day">
            <div class="border-conners-left-top"></div>
            <div class="border-conners-left-bottom"></div>    
            <div class="border-conners-right-top"></div>    
            <div class="border-conners-right-bottom"></div> 
            <div class="hotgames-header clearfix">
              <span class="hotgames-header-title clearfix">24小时流行榜：</span>
              <a class="more" target="_blank" href="http://game.renren.com/?rrlobby_web">更多&gt;&gt;</a>
            </div>
            <div class="hotgames-body clearfix">
              <ul>
                <c:forEach var="game" items="${gameRankList}" varStatus="status">
                <c:if test="${status.index == 0}">
                <li>
                  <div class ="top1">
                    <div class="hover clearfix" style="display: block;">
                      <span class="num">1</span>
                      <div class="app-pic">
                        <a target="_blank" href="${game.appLink}" title="${game.appName}"  class="app-pica">
                        <img width="50" height="50" src="${game.appLogo}" alt="${game.appName}">
                        <div class="mask"></div>
                        </a>
                      </div>
                      <div class="app">
                        <div class="name">
                          <span class="name"><a target="_blank" href="${game.appLink}" title="${game.appName}">${game.appName}</a>
                          </span>
                        </div>
                        <div class="wrapper" title="${game.rating}分，基于${game.comment_account}人评价">
                          <ul class="star-wrap">
                            <li class="" style="width:${game.rating * 17}px;"></li>
                          </ul>
                        <span class="score">${game.rating}</span>
                        </div>
                        <span>${game.cate}</span>  
                        </div>
                      <img width="11" height="11" class="arrow <c:if test="${game.upOrDown >= 0}">up</c:if><c:if test="${game.upOrDown < 0}">down</c:if>" src="http://a.xnimg.cn/a.gif" alt="<c:if test="${game.upOrDown >= 0}">上升</c:if><c:if test="${game.upOrDown < 0}">下降</c:if>" title="<c:if test="${game.upOrDown >= 0}">上升</c:if><c:if test="${game.upOrDown < 0}">下降</c:if>">
                    </div>
                  </div>
                </li>
                </c:if>
                <c:if test="${status.index != 0}">
                <li>                  
                  <div class="top${status.index + 1}">  
                    <div class="nor clearfix" style="display: block;">
                      <span class="num">${status.index + 1}</span>
                      <img class="icon" title="${game.appName}" src="${game.appIcon}" alt="">
                      <a class="name" target="_blank" href="${game.appLink}">${game.appName}</a>
                      <img width="11" height="11" class="arrow <c:if test="${game.upOrDown >= 0}">up</c:if><c:if test="${game.upOrDown < 0}">down</c:if>" src="http://a.xnimg.cn/a.gif" alt="<c:if test="${game.upOrDown >= 0}">上升</c:if><c:if test="${game.upOrDown < 0}">下降</c:if>" title="<c:if test="${game.upOrDown >= 0}">上升</c:if><c:if test="${game.upOrDown < 0}">下降</c:if>">
                    </div>
                  </div>
                </li>
                </c:if>
                </c:forEach>
              </ul>
            </div>
        </div>
     </div>
</div>
</body>
<script type="text/javascript">
   var num = 0,list = document.getElementsByName("numLink"),array = [{img:"http://gadget.talk.renren.com/images/christmas.jpg",link:"http://app.renren.com/activity/christmas?rrlobby_top1christmas",info:"人人城市"},{img:"http://gadget.talk.renren.com/images/top2xiaoleyuan.png",link:"http://apps.renren.com/xiaoleyuan?rrlobby_top2",info:"小小乐园"},{img:"http://gadget.talk.renren.com/images/top3lstx.jpg",link:"http://lstx.renren.com?rrlobby_top3",info:"乱世天下"},{img:"http://gadget.talk.renren.com/images/top4ztx.jpg",link:"http://wan.renren.com/ztx?rrlobby_top4",info:"战天下"},];
   
   for(var i=0;i<list.length;i++){
       list[i].onmouseover = function(e){
          var e = e ? e :window.event;
          if(e.stopPropagation)
              e.stopPropagation();
          else
              e.cancelBubble = true;
	      if(timeId)
		     clearInterval(timeId);
			 
		  for(var i=0;i<list.length;i++){
			 list[i].className = "";
		  }
		  this.className = "cur";
		  var x = Number(this.innerHTML) - 1;
		  document.getElementById("appImg").style.backgroundImage = "url(" + array[x].img + ")";
 
          var url = array[x].link;
          document.getElementById("appImg").onclick = function(){
             window.open(url);
          };
          return false;
	   }
   }
   
   function loop(){
       if(num == 4)
          num = 0;
       document.getElementById("appImg").style.backgroundImage = "url(" + array[num].img + ")";    
       var url = array[num].link;
       document.getElementById("appImg").onclick = function(){
           window.open(url);
       }
	   for(var i=0;i<list.length;i++){
	      list[i].className = "";
	   }
	   list[num].className = "cur";
       num++;
	   window.timeId = setTimeout(loop,5000);
   }
   
   window.onload = function(){
       loop();
	   
   };
</script>
</html>


