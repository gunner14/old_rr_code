<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!doctype html public "-//w3c//dtd xhtml 1.0 transitional//en" "http://www.w3.org/tr/xhtml1/dtd/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=gb2312" />
<meta http-equiv="content-language" content="zh-cn" />
<title>Mini App</title>
<script type="text/javascript">
<!--
var IE=false,FF=false,W=window,D=document,H,B,GET="getElementsByTagName",GEI="getElementById",qq=0;
function fold(){
var e;
e=fixE(e);
if(e)element=fixElement(e);
element=element.parentNode.parentNode;
element.className=element.className.indexOf("hide")>0?"module":"module hide";
}
var Drag={
draging : false,
x : 0,
y : 0,
element : null,
fDiv : null,
ghost : null,
addEvent : function(){var a=D[GET]("li");for(var i=a.length-1;i>-1;i--)if(a[i].className=="module")a[i].onmousedown=Drag.dragStart;},
ix:2,iy:7,
ox:6,oy:7,
fx:6,fy:6,
dragStart : function (e){
   if(Drag.draging)return;
   var e;
   e=fixE(e);
   if(e)element=fixElement(e);

   /*********
   var k,s="";
   for(k in element)s+=k+" : "+element[k]+"<br/>";
   D.getElementById("bbb").innerHTML=s;
   **********/
   D.getElementById("aaa").innerHTML=element.parentNode.offsetTop+ ","+element.parentNode.offsetHeight;
   //测试
   if(element.className!="title")return;
   element=element.parentNode;
   Drag.element=element;
   //以上获得当前移动的模块
   Drag.x=e.layerX?e.layerX+Drag.fx:(IE?e.x+Drag.ix:e.offsetX+Drag.ox);
   Drag.y=e.layerY?e.layerY+Drag.fy:(IE?e.y+Drag.iy:e.offsetY+Drag.oy);
   //鼠标相对于模块的位置
   Drop.measure();
   if(e.layerX){Drag.floatIt(e);Drag.drag(e);}//fix FF
   B.style.cursor="move";
   D.onmousemove=Drag.drag;
   D.ondragstart=function(){window.event.returnValue = false;}
   D.onselectstart=function(){window.event.returnValue = false;};
   D.onselect=function(){return false};
   D.onmouseup=element.onmouseup=Drag.dragEnd;
   element.onmousedown=null;
},
drag : function (e){
   var e;
   e=fixE(e);
   if(!Drag.fDiv)Drag.floatIt(e);//for IE & Opera
   var x=e.clientX,y=e.clientY;
   Drag.fDiv.style.top=y+H.scrollTop-Drag.y+"px";
   Drag.fDiv.style.left=x+H.scrollLeft-Drag.x+"px";
   Drop.drop(x,y);
   //statu(e);
},
dragEnd : function (e){
   B.style.cursor="";
   D.ondragstart=D.onmousemove=D.onselectstart=D.onselect=D.onmouseup=null;
   Drag.element.onmousedown=Drag.dragStart;
   if(!Drag.draging)return;
   Drag.ghost.parentNode.insertBefore(Drag.element,Drag.ghost);
   Drag.ghost.parentNode.removeChild(Drag.ghost);
   B.removeChild(Drag.fDiv);
   Drag.fDiv=null;
   Drag.draging=false;
   Drop.init(D[GEI]("container"));
},
floatIt : function(e){
   var e,element=Drag.element;
   var ghost=D.createElement("LI");
   Drag.ghost=ghost;
   ghost.className="module ghost";
   ghost.style.height=element.offsetHeight-2+"px";
   element.parentNode.insertBefore(ghost,element);
   //创建模块占位框
   var fDiv=D.createElement("UL");
   Drag.fDiv=fDiv;
   fDiv.className="float";
   B.appendChild(fDiv);
   fDiv.style.width=ghost.parentNode.offsetWidth+"px";
   fDiv.appendChild(element);
   //创建容纳模块的浮动层
   Drag.draging=true;
}
}
var Drop={
root : null,
index : null,
column : null,
init : function(it){
   if(!it)return;
   Drop.root=it;
   it.firstItem=it.lastItem=null;
   var a=it[GET]("ul");
   for(var i=0;i<a.length;i++){
    if(a[i].className!="column")continue;
    if(it.firstItem==null){
     it.firstItem=a[i];
     a[i].previousItem=null;
    }else{
     a[i].previousItem=a[i-1];
     a[i-1].nextItem=a[i];
    }
    a[i].nextItem=null;
    it.lastItem=a[i];
    a[i].index=i;
    a[i].firstItem=a[i].lastItem=null;
    var b=a[i][GET]("li");
    for(var j=0;j<b.length;j++){
     if(b[j].className.indexOf("module")==-1)continue;
     if(a[i].firstItem==null){
      a[i].firstItem=b[j];
      b[j].previousItem=null;
     }else{
      b[j].previousItem=b[j-1];
      b[j-1].nextItem=b[j];
     }
     b[j].nextItem=null;
     a[i].lastItem=b[j];
     b[j].index=i+","+j;
    }
   }
},
measure : function(){
   if(!Drop.root)return;
   var currentColumn=Drop.root.firstItem;
   while(currentColumn){
    var currentModule=currentColumn.firstItem;
    while(currentModule){
     currentModule.minY=currentModule.offsetTop;
     currentModule.maxY=currentModule.minY+currentModule.offsetHeight;
     currentModule=currentModule.nextItem;
    }
    currentColumn.minX=currentColumn.offsetLeft;
    currentColumn.maxX=currentColumn.minX+currentColumn.offsetWidth;
    currentColumn=currentColumn.nextItem;
   }
   Drop.index=Drag.element.index;
},
drop : function(x,y){
   if(!Drop.root)return;
   var x,y,currentColumn=Drop.root.firstItem;
   while(x>currentColumn.maxX)if(currentColumn.nextItem)currentColumn=currentColumn.nextItem;else break;
   var currentModule=currentColumn.lastItem;
   if(currentModule)while(y<currentModule.maxY){
    if(y>currentModule.minY-12){
     if(Drop.index==currentModule.index)return;
     Drop.index=currentModule.index;
     if(currentModule.index==Drag.element.index){if(currentModule.nextItem)currentModule=currentModule.nextItem;else break;}
     currentColumn.insertBefore(Drag.ghost,currentModule);
     Drop.column=null;
     window.status=qq++;
     return;
    }else if(currentModule.previousItem)currentModule=currentModule.previousItem;else return;
   }
   if(Drop.column==currentColumn.index)return;
   currentColumn.appendChild(Drag.ghost);
   Drop.index=0;
   Drop.column=currentColumn.index;
   window.status=qq++;
}
}
var webNote={
obj : null,
canEdit : function(e){
   var e,element;
   e=fixE(e);
   element=fixElement(e);
   if(element.className!='webNote')return;
   if(typeof element.contentEditable!="undefined"){
    element.contentEditable=true;
    element.style.borderColor='red';
    element.focus();
    webNote.obj=element;
   }
},
cannotEdit : function(){
   if(!webNote.obj)return;
   if(typeof webNote.obj.contentEditable!="undefined"){
    webNote.obj.style.borderColor='#ffffe0';
    webNote.obj.contentEditable=false;
    webNote.obj=null;
   }
}
}
function fixE(e){var e;e=e?e:(window.event?window.event:null);return e}
function fixElement(e){var e;return e.target?(e.target.nodeType==3?e.target.parentNode:e.target):e.srcElement;}
onload=function(){
B=D[GET]("body")[0];
H=D[GET]("html")[0];
Drop.init(D[GEI]("container"));
Drag.addEvent();
}
function statu(e){
var e,element;
element=fixElement(e);
var aa=D.getElementById("aaa");
aa.innerHTML="e.xy:("+e.x+","+e.y+")<br/>e.offsetXY:("+e.offsetX+","+e.offsetY+")<br/>e.clientXY:("+e.clientX+","+e.clientY+")<br/>element.offsetLeftTop:("+element.offsetLeft+","+element.offsetTop+")<br/>e.layerXY:("+e.layerX+","+e.layerY+")";
}
//-->
</script>
<style type="text/css">
body,table,td,th,input,textarea,button,select{font:13px/16px Verdana,"宋体",sans-serif;}
table{border-collapse:collapse;}
p{margin:0px;}
.container{margin:8px;}
.column{width:33%;margin:0px;padding:0px;float:left;overflow:hidden;}
.module{margin:5px;padding:0px;border:1px solid #3B5998;position:relative;background-color:white;list-style:none;}
.title{background-color:#E3EEF8;padding:1px 0px;width:100%;overflow:hidden;cursor:move;z-index:0;font-weight:bold;}
.cont{padding:3px;overflow:hidden;}
.hide .cont{display:none;}
.pageTitle{font-weight:bold;text-align:center;}
input.pageTitle{display:none;border:0px;padding:0px;width:100%;}
.webNote{background-color:#ffffe0;border:1px solid #ffffe0;}
textarea.webNote{display:none;overflow:hidden;padding:0px;border:0px;padding:0px;}
.ghost{border:1px dashed red;}
.float{position:absolute;z-index:100;margin:0px;padding:0px;overflow:hidden;list-style:none;-moz-opacity:.75;filter:Alpha(opacity=75);}
</style>
<!--[if IE]>
<script type="text/javascript">
IE=true;
</script>
<style type="text/css">
input.pageTitle{margin:-1px;}
</style>
<![endif]-->
<![if !IE]><![endif]>
</head>
<body>
<center><span class="pageTitle" onclick="with(this.nextSibling){value=this.innerHTML;style.display='block';focus()};this.style.display='none';">[${hostid}] You can input a topic here!</span><input class="pageTitle" onblur="with(this.previousSibling){D.title=innerHTML=this.value?this.value:'Demo';style.display='block';};this.style.display='none'" /></center>
<div class="container" id="container">
<ul class="column">
	<c:forEach var="i" items="${records}">
	<li class="module">
		<div class="title"><span onclick="fold()">+</span>id:[${i.id}] - 应用编号：${i.appid}
		<c:choose>
		    <c:when test="${!i.delmark}">
		    <a href="/demo/del?id=${i.id}">删除</a>
		    <a href="/demo/get?id=${i.id}">Get</a>
		    </c:when>
		    <c:otherwise>
		    <span>已删除</span>
		    </c:otherwise>
		</c:choose>
		</div>
		<div class="cont"><div class="webNote" onblur="webNote.cannotEdit()" onclick="webNote.canEdit(event)">${i.content}</div><textarea class="webNote"></textarea></div>
		<div style="border:1px dotted green;margin:5px;">
		  <form action="/demo/set" style="margin:0px;">
                 <c:out value="${i.position}" />
                 <c:out value="${i.config}" />
             <input name="content" value="${i.content}" />
             <input name="id" type="hidden" value="${i.id}" />
                 <input type="submit" value="修改" />
                 
          </form>
		</div>
	</li>
	</c:forEach>
	<li class="module">
		<div class="title"><span onclick="fold()">+</span> 所有应用</div>
		<div class="cont">${i.content}
		<c:forEach var="i" items="${dicts}">
		    [<c:out value="${i.app_id}" />]
		    <font color=red><c:out value="${i.app_name}" /></font> 
		    <font color="green"><c:out value="${i.app_description}" /></font>
		    <a href="/demo/add?app_id=${i.app_id}">添加</a>
		    <a href="/demo/appinfo?app_id=${i.app_id}">查看</a>
		    <br />
		</c:forEach>
		</div>
	</li>
</ul>
<ul class="column">
	<li class="module">
	    <div class="title"><span onclick="fold()">+</span> Coodinate</div>
	    <div class="cont" id="aaa">content</div>
	   </li>
	</ul>
<ul class="column">
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> 占位置的</div>
    <div class="cont">后死诸君多努力<br/>捷报飞来<br/>当纸钱<br /><br /><br />
    <span>
    <img title="千橡互动" height="194" alt="千橡互动" width="415" src="http://www.oakpacific.com/zh/images/qiantai.jpg" />
    千橡互动集团是集沟通社交平台、娱乐互动门户、网络游戏及社交化电子商务等多元化业务为一身的综合性互联网集团公司，通过多个Web2.0网站向广大用户提供包括SNS网站、门户网站、信息化互动社区、网络游戏和社交化电子商务网站等在内的一系列服务。自2002年11月公司成立以来，经过多年的发展，千橡互动已成为中国互联网行业里最具影响力的企业之一。

作为Web2.0理念的积极倡导者与实践者，千橡互动集团致力于积极促进人际沟通娱乐方式的革命， 让世界上每一个人： 沟通更有效，娱乐更丰富，联系更紧密。
    </span>
    </div>
   </li>
</ul>
<!-- 
<ul class="column">
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> Coodinate</div>
    <div class="cont" id="aaa">content</div>
   </li>
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> contentEditable</div>
    <div class="cont" onclick="alert('this.contentEditable:'+this.contentEditable)">click here</div>
   </li>
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> long long ago</div>
    <div class="cont">有形者 生于无有 有归于无<br/>是以 凡妄<br/>若是能见诸相非</div>
   </li>
</ul>
<ul class="column">
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> this.parentNode</div>
    <div class="cont"><button onclick="alert(this.parentNode.parentNode.parentNode.innerHTML)">parentNode</button></div>
   </li>
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> webNote</div>
    <div class="cont"><div class="webNote" onblur="webNote.cannotEdit()" onclick="webNote.canEdit(event)">You can note something here.<br/><br/>从前有座山</div><textarea class="webNote"></textarea></div>
   </li>
   <li class="module">
    <div class="title"><span onclick="fold()">+</span> Google</div>
    <div class="cont"><center><a href="http://www.google.com" style="font:bold 31px/2 Arial;">Google</a></center></div>
   </li>
</ul>
-->
</div>
<div style="font-size:10px;line-height:14px;clear:both;margin:6px 3%;text-align:center;border:1px solid #eee;">inshion</div>
</body>
</html>





