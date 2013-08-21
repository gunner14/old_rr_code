<%@ page contentType="text/html;charset=UTF-8"%>
<style type="text/css">   
#username {   
   font-size:12px;
   font-family:tahoma;
   float:left;
   width:240px;
   height:54px;   
   line-height:50px;
   border:1px solid #005EAC;   
   background-color:#F3FAFF;  
   padding-left:20px; 
 }   
 
 #zombname {   
   font-size:12px;
   font-family:tahoma;
   float:right;
   width:240px;
   height:54px;   
   line-height:50px;
   border:1px solid #005EAC;   
   background-color:#F3FAFF;  
   padding-left:20px; 
 }  
 
 #userhead{
    float:left;
    text-align: center;
    border: 1px dotted green;
    padding:2px;
    width:75px;
    height:75px;
 }
</style>
<div style="height: 85px;">
    <div id="userhead">
    <img src="${visiter.tinyFullUrl }" />
    </div>
    <div id="username">
    <span>Hello:${visiter.name } 欢迎使用 人人 跳跳乐</span>
    </div>
    <div style="float:right;border: 1px dotted red;padding:2px;">
    <img src="http://app.xnimg.cn/application/logo/20100914/14/35/LnUn65b018153.gif" />
    </div>
    <div id="zombname">
    <span><font color="red"><b>跳跳有风险 操作需谨慎!</b></font></span>
    </div>
</div>

