<%@ page contentType="text/html;charset=UTF-8" session="false" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta charset="utf-8">
<title>人人游戏—用户资料补全</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link href="http://wan.renren.com/css/common.css" rel="stylesheet" type="text/css" />
<link type="text/css" rel="stylesheet" href="http://wan.renren.com/fillinfo/css/user_info.css">
<script type="text/javascript" src="http://wan.renren.com/js/jquery.js"></script>
<script type="text/javascript" src="http://wan.renren.com/js/user.js"></script>
<script type="text/javascript" src="http://wan.renren.com/fillinfo/css/fillinfo.js"></script>
<style>
.errors_div {
    background: none repeat scroll 0 0 #FFEBE8;
    border: 1px solid #DD3C10;
    margin-bottom: 15px;
    padding: 5px 0;
    text-align: center;
    width: 495px;
}
</style>
</head>

<body>
<div id="doc-w"> 
  <!--{{ 人人游戏通用header-->
  <div class="header"> 
    <script type="text/javascript" src="http://wan.renren.com/js/s_head.js"></script> 
    <script type="text/javascript">
   $("#head_userlogin").html("<a href=\"javascript:cookie.clear();window.location.href='http://wan.renren.com/';\">退出</a>");
   $("#head_userreg").html("<a href='http://wan.renren.com/'>返回人人游戏</a>");

   </script> 
  </div>
  <!--{{ 完善个人信息-->
  <div id="contaniner">
    <div id="main">
      <div class="userinfo-form">
        <h2>完善个人信息，交友游戏两不误</h2>
        <form method="post" action="http://guide.renren.com/fillnames" name="userinfoform" id="userinfoform">
        <div class="errors_div" <c:if test="${empty guide_lead_erromsg || fn:trim(guide_lead_erromsg) eq '验证码错误' }">style="display:none"</c:if>><p>${guide_lead_erromsg}</p></div>
          <div class="field">
            <label for="name">真实姓名:</label>
            <div class="inputbox">
              <input class="name" type="text" id="username" name="username" value="" onBlur="hid('real_name_err');if(this.value!='')checkTrueName(checkok,checkfail);" onClick="hid('real_name_ok');dis('real_name_err')"/>
              <span class="right real_name_ok" style="display:none;"><img src="http://wan.renren.com/fillinfo/images/fzcg_dh.gif" /></span>
              <div style="display:none;" class="error real_name_err">

                <div class="error-outer">
                  <div class="error-inner">
                    <p>请输入<b>真实中文姓名</b>，方便朋友查找</p>
                  </div>
                </div>
              </div>
            </div>

          </div>
          <div class="field">
            <label>性别:</label>
            <div class="inputbox">
              <label class="labelRadio">
                <input name="gender" class="gender" value="男生" type="radio" />
                男</label>
              <label class="labelRadio">

                <input name="gender" class="gender" value="女生" type="radio" />
                女</label>
              <div style="display:none" class="error-red sexerr">
                  <div class="error-outer-red">
                    <div class="error-inner-red">
                      <p>请选择性别</p>
                    </div>
                  </div>

              </div>
            </div>
          </div>
          <div class="field">
            <label>生日:</label>
            <div class="inputbox birthwidth">
            <div style="float:left;padding-right:24px;height:30px">
              <select onchange="onYearChange()" id="useryear" name="useryear">
                <option value="">请选择</option>
                <c:set var="strutsOptionYearComponent_begin" value="2012" />
                <c:set var="strutsOptionYearComponent_end" value="1900" />
                <%@ include file="/inc/strutsOptionBirthdayYearComponent.inc.jsp"%>
              </select> 年
              <select onchange="onMonthChange()" id="usermonth" name="usermonth">
                <option value="">--</option>
                <c:set var="strutsOptionYearComponent_begin" value="12" />
                <c:set var="strutsOptionYearComponent_end" value="1" />
                <%@ include file="/inc/strutsOptionYearComponent.inc"%>
              </select> 月 <span id="c_userday">
		<select onchange="onSubmit(openNextButton);" id="userday" name="userday">
		<option value="">--</option>
                <c:set var="strutsOptionYearComponent_begin" value="31" />
                <c:set var="strutsOptionYearComponent_end" value="1" />
                <%@ include file="/inc/strutsOptionYearComponent.inc"%>
		</select></span> 日
              </div>
	            <div style="display:none" class="error-red birtherr">
	              <div class="error-outer-red">
	                <div class="error-inner-red">
	                  <p>请填写生日</p>
	                </div>
	              </div>
	            </div>
	            <p class="b_tip"><a href="javascript:;" onClick="dis('birthtips');">为什么要填写我的生日？</a></p>
              </div>
             </div>
             
<%
java.util.Random random = new java.util.Random();
int id = random.nextInt(1431655764);
%>
             <div class="field pad-L">
             <img id="verifyPic" src="http://icode.${applicationScope.sysdomain}/getcode.do?t=register_<%=id%>&rk=600&rnd=<%=System.currentTimeMillis()%>" align="absmiddle" /> <a href="#">看不清楚换一张</a>
             </div>
           <div class="field">
            <label for="valicode">验证码:</label>
            <div class="inputbox">
              <input class="valicode" type="text" id="valicode" name="icode" maxlength="6" size="5" value="" onBlur="" onClick=""/>
              <input type="hidden" name="key_id" id="key_id" value=<%=id%> />
              <span class="right valicode_name_ok" style="display:none;"><img src="http://wan.renren.com/fillinfo/images/fzcg_dh.gif" /></span>
              <div <c:if test="${empty guide_lead_erromsg || fn:trim(guide_lead_erromsg) !eq '验证码错误'}">style="display:none;"</c:if> class="error valicode_err">
                <div class="error-outer">
                  <div class="error-inner">
                    <p>您输入的验证码不正确</p>

                  </div>
                </div>
              </div>
            </div>
           </div>
          
            <div class="field pad-L">
              <input class="submit-btn nextsteps" type="button" onClick="return onSubmit();" value="下一步" />
              <input class="submit-btn oversteps" type="button" onClick="window.location.href='http://wan.renren.com/';" value="跳过此步骤"/>
            </div>

        </form>
        <script type="text/javascript">
	        var username = '${username}';
	        var gender = '${gender}';
	        var useryear = '${useryear}';
	        var usermonth = '${usermonth}';
	        var userday = '${userday}';
	        var icode = '${icode}';
        </script>
      </div>
    </div>
    <div id="sidebox"></div>
  </div>
  <!--{{ 人人游戏通用footer-->
  <div id="footer"> 
    <script type="text/javascript" src="http://wan.renren.com/js/s_foot.js"></script> 
  </div>

</div>
<div class="birthtips" style="display:none;position: absolute; top: 0px; left: 0px; right: 0px; bottom: 0px; height: 100%; width: 100%;z-index:1000;">
  <div style="display: block; width: 400px; margin: 217px auto 0pt;">
    <table class="pop_dialog_table" style="width: 100%; height: 100%;">
      <tbody>
        <tr>
          <td class="pop_topleft"></td>
          <td class="pop_border"></td>
          <td class="pop_topright"></td>
        </tr>

        <tr>
          <td class="pop_border"></td>
          <td id="pop_content" class="pop_content"><div id="pop_content_box_wrap" style="position:relative">
              <div style="top:0;left:0;z-index:600;width:100%" id="pop_content_box">
                <h2><span id="ui_dialog_header">为什么需要提供我的生日日期？</span></h2>
                <div class="dialog_content">
                  <div class="dialog_body" id="ui_dialog_body">出于安全考虑，同时为了保证人人网用户的真实性，凡注册人人网用户都需要填写真实的出生日期。<br>
                    <span class="gray">你可以在个人主页上隐藏你的出生日期。</span></div>

                  <div class="dialog_buttons" id="ui_dialog_footer">
                    <input type="button" class="input-submit" id="ui_dialog_button_confirm" value="确定" onClick="hid('birthtips')">
                  </div>
                </div>
              </div>
            </div></td>
          <td class="pop_border"></td>
        </tr>
        <tr>

          <td class="pop_bottomleft"></td>
          <td class="pop_border"></td>
          <td class="pop_bottomright"></td>
        </tr>
      </tbody>
    </table>
  </div>
</div>
<iframe name="iproxy" id="iproxy" src="http://reg.renren.com/ajaxproxy.htm" height="0" width="0" frameborder="0"></iframe>

</body>
</html>

