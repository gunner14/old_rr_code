<%@ page contentType="text/html;charset=UTF-8"%>
<style type="text/css">   
#username {   
   font-size:14px;
   font-family:tahoma;
   width:99%;
   height:50px;
   line-height:50px;
   border:1px solid #005EAC;
   background-color:#F3FAFF;
   padding:2px;
}   

</style>

<div style="height:60px;">
    <div id="username">
    <img src="${userTinyurl}" />
    <span>&nbsp;&nbsp;当前操作管理员：${userName}&nbsp;[${userId}]</span>&nbsp;&nbsp;
    <span style="color:#ff0000">活动时间勿频改，自动控制上下线；删除操作有风险，求助请找管理员！</span>
    </div>
    <br />
</div>



