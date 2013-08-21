<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head><!-- _fi_gum_ ${lead_type_flag} -->
<c:if test="${lead_type_flag eq '_ABF' && ((visiter.status == 13)||( empty visiter.name || visiter.name eq ''))}"><%response.sendRedirect("http://zhan.renren.com/reg"); %></c:if>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><meta name="Description" content="人人,人人网是一个真实的社交网络，联络你和你周围的朋友。加入人人,人人网你可以:联络朋友，了解他们的最新动态；和朋友分享相片、音乐和电影；找到老同学，结识新朋友；用照片和日志记录生活,展示自我。" />
	<meta name="Keywords" content="Xiaonei,人人,人人网,大学,同学,同事,白领,个人主页,博客,相册,群组,社区,交友,聊天,音乐,视频,校园,人人,人人网" />
	<title>${domainIdentified["siteName_fillinfo"]} - 寻找你认识的人</title><link href="http://s.xnimg.cn/favicon-rr.ico" rel="shortcut icon" type="image/x-icon" />
	<link href="http://s.xnimg.cn/renren_logo_apple.png" rel="apple-touch-icon" />
	<!-- <script type="text/javascript" src="http://s.xnimg.cn/js/register/register-app-invite.js?ver=$revxxx$"></script> -->
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<script type="text/javascript" src="http://s.xnimg.cn/imgpro/msntab/scripts/swfobject.js?ver=$revxxx$"></script>

	<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.guide.darkcoral.js?ver=$revxxx$"></script>

	<link href="http://s.xnimg.cn/csspro/apps/guide-reg.css?ver=$revxxx$" rel="stylesheet" rev="stylesheet" media="all" />
</head>
<body>
	<script type="text/javascript">
		var Guide_reg_stage = ${host.stage};
		var guideTrace_pageId = 'p11';
		var butuiren = '${host.name}' == '' ? true:false ;
	</script>
	<div class="main_wrap clearfix">
		<div class="reg_logo">10秒找到你所有朋友</div>
		<div class="guide-main-new clearfix">
			<span class="login"><a href="http://www.renren.com/Logout.do" >退出</a></span>
			<div class="steps first clearfix">
				<p class="online">1.填写个人信息</p>

				<p>2.找到朋友，完成</p>
			</div>
			<!--stage:${host.stage}-->
			<div id="" class="guide-content clearfix">
				<div class="main-data clearfix">

					<!-- 手机注册上来的引导信息 -->
					<c:if test="${empty visiter.name  || visiter.name=='' || empty visiter.gender || visiter.gender==''}">
					<table>
						<c:if test="${empty visiter.name  || visiter.name==''}">
						<tr class="candyPaper" id="swork_mid_entary_tr" >
                            <td class="td-name"><em>*</em>我的名字 : </td>
                            <td>
                                <input type="text" class="long input-text" id="user_name" name="user_name" autocomplete="off" />
                                <p class="green-tip bottomTip" style="visible:hidden;">请输入真实中文姓名</p>
                            </td>
							<td></td>
                        </tr>
                        </c:if>
                        <c:if test="${empty visiter.gender  || visiter.gender==''}">
                        <tr class="candyPaper" id="swork_mid_entary_tr" >
                            <td class="td-name"><em>*</em>性别 : </td>
                            <td>
                                <label for="male"><input type="radio" value="男生" id="male" name="gender">男</label>
								<label for="female"><input type="radio" value="女生" id="female" name="gender">女</label>
                                <p class="green-tip bottomTip" style="visible:hidden;">请选择性别</p>
                            </td>
							<td></td>
                        </tr>
                        </c:if>
                    </table><!-- End:手机引导 -->
                    </c:if>

					<!-- 资料表单:一大砣表单 -->
					<table id="stage">
						<tr>
							<td class="td-name"><img src="http://xnimg.cn/imgpro/icons/yes.png"/>我现在 : </td>
							<td>

								<select id="selectStage" class="select long" id="">
									<option value="work_form" >已经工作了</option>
									<option value="univ_form">正在上大学</option>
									<option value="mid_form">正在上中学</option>
									<option value="none_form">是其他身份</option>
								</select>
								<!--<p class="green-tip">请用visibility:hidden;将提示信息隐藏</p>请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
							</td>
							<td></td>
						</tr>
					</table>
					
					<!-- 我上大学了 -->
					<form  id="univ_form" method="post" action="fillinfos-dx?f=l" style="display:none;">
					<input type="hidden" value="20" name="stage"/>
					<input type="hidden" value="20" name="formDaxue.stage" />
					<table>
						<tr class="candyPaper">
							<td class="td-name"><em>*</em>我的大学:</td>
							<td class="schoolbox">
								<input type="text" class="mid input-text  s-select s-univ" name="formDaxue.universityname" id="daxue_univ" schema="univName"/>
								<input type="hidden" id="daxue_univ_id" name="formDaxue.universitycode" value="0"/>
								<select class="select select-gray" id="univ_year" name="formDaxue.universityyear" schema="univYear" ><option value="">入学年份</option>
	                                <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                <%@ include file="inc/years.inc" %>
                               	</select>

								<p>
									<select class="mid select select-gray univ_dept_select" id="daxue_univ_dept" name="formDaxue.universitydepartment" schema="univDepart">
										<option value="">请选择院系</option>
									</select>
									<select class="short select " id="univ_type" name="formDaxue.coursetype"  schema="univCourse">
										<option value="">请选择类型</option>
										<option value="大学生">大学生</option>
										<option value="硕士">硕士</option>
										<option value="博士">博士</option>
										<option value="其他">其他</option>
									</select>
								</p>
								<p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
							</td>

							<td><a class="arrow-btn" href="#nogo"></a></td>
						</tr>
						<tr class="candyPaper">					
							<td class="td-name"><em>*</em>我的高中:</td>
							<td>
								<input type="text" class="mid input-text s-select s-high" id="univ_high" name="formDaxue.highschoolname" schema="highName"/>
								<input type="hidden" value="0" id="univ_high_id" name="formDaxue.highschoolcode"/>
								<select class="short select select-gray" id="univ_high_year" name="formDaxue.highschoolyear"  schema="highYear">
                                   <option value="">入学年份</option>
                                       <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                       <c:set var="strutsOptionYearComponent_end" value="1900" />
                                       <%@ include file="inc/years.inc"%>
                               	</select>
								<p class="green-tip bottomTip" style="visibility:hidden">提示文案请用visibility:visible;显示</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
							</td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
						</tr>
						<tr class="btn-box">
							<td></td>
							<td><input type="submit" value="保存我的个人信息" class="input-submit" /></td>
							<td></td>
						</tr>
					</table>
					</form>
					<!-- End:我上大学了 -->
					
					<!-- 我上中学 -->
					<form id="mid_form" method="post" action="fillinfos-zx?f=l" style="display:none;">
					<input type="hidden" value="10" name="stage"/>
					<table>
						<tr class="candyPaper">
							<td class="td-name"><em>*</em>我的学校 :</td>
							<td class="mid-school" width="190">
								<p>
									<label><input type="radio" bind="formZhongxue.highschoolname;formZhongxue.juniorhighschoolname" name="formZhongxue.schooltype" value="0" schema="midSchool" />高中</label>
									<label><input type="radio" bind="formZhongxue.juniorhighschoolname;formZhongxue.elementaryschoolcode" name="formZhongxue.schooltype" value="1" schema="midSchool" />初中</label>
									<label><input type="radio" bind="formZhongxue.techschoolname;formZhongxue.juniorhighschoolname" name="formZhongxue.schooltype" value="2" schema="midSchool"/>中专技校</label>
								</p>
								<p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
							</td>
							<td></td>
						</tr>
						
						<tr class="candyPaper" id="mid_high_tr" style="display:none;">
                            <td class="td-name"><em>*</em>高中 : </td>
                            <td>
								<p>    
									<input type="text" class="mid input-text  s-select s-high" name="formZhongxue.highschoolname" id="mid_high" schema="highName"/> 
									<input type="hidden" name="formZhongxue.highschoolcode" id="mid_high_id" value="0"/>
									<select class="select select-gray" name="formZhongxue.highschoolyear" id="mid_high_year" schema="highYear">
									<option value="">入学年份</option>
										<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
										<c:set var="strutsOptionYearComponent_end" value="1900" />
										<%@ include file="inc/years.inc" %>
									</select>
								</p>
								<p class="banji">
									高一<input type="text" id="grade1" class="input-text digital" name="formZhongxue.highschoolclass1"> 班　高二<input type="text" id="grade2" class="input-text digital" name="formZhongxue.highschoolclass2"> 班　高三<input type="text" id="grade3" class="input-text digital" name="formZhongxue.highschoolclass3"> 班
								</p>
								<p class="green-tip bottomTip" style="visibility:hidden">请填写高中，会找到高中同学</p>
                            </td>
                            <td><a href="#nogo" class="arrow-btn"></a></td>
                        </tr>
                        
						<tr class="candyPaper" id="mid_tech_tr" style="display:none;">
                            <td class="td-name"><em>*</em>中专技校 : </td>
                            <td> 
                                <input type="hidden" value="10" name="formZhongxue.stage" />
                                <input type="text" class="mid input-text  s-select s-tech" name="formZhongxue.techschoolname" schema="techName"/> 
                                <input type="hidden" id="mid_tech_id" value="0" name="formZhongxue.techschoolcode"/>
                                <select class="select select-gray"  name="formZhongxue.techschoolyear"  id="mid_tech_year" schema="techYear">
                                	<option value="" >入学年份</option>
	                                <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                <%@ include file="inc/years.inc" %>
                                </select>
                                <p class="green-tip bottomTip" style="visibility:hidden">请填写中专技校，会找到中专技校同学</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>
                        
						<tr class="candyPaper" style="display:none;">					
							<td class="td-name"><em>*</em>初中:</td>
							<td>
								<input type="text" class="input-text s-select s-junior mid" id="mid_junior" name="formZhongxue.juniorhighschoolname" schema="juniorName"/>
                               	<input type="hidden" value="0" id="mid_junior_id" name="formZhongxue.juniorhighschoolcode" />
								<select class="select select-gray" name="formZhongxue.juniorhighschoolyear" id="mid_junior_year" schema="juniorYear">
                                    <option value="">入学年份</option>
                                    <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                    <c:set var="strutsOptionYearComponent_end" value="1900" />
                                    <%@ include file="inc/years.inc" %>
                               	</select>
								<p class="green-tip bottomTip" style="visibility:hidden">请填写初中，会找到初中同学</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
							</td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
						</tr>
						<tr class="candyPaper" style="display:none;">					
							<td class="td-name"><em>*</em>小学:</td>
							<td>
								<input type="text" class=" mid input-text" id="mid_entary" autocomplete="off" name="formZhongxue.elementaryschoolname" schema="entaryName"/>
                                <input  type="hidden" id="mid_entary_id" name="formZhongxue.elementaryschoolcode"/>
								<select class="select select-gray" id="mid_entary_year" name="formZhongxue.elementaryschoolyear" schema="entaryYear">
	                                <option value="">入学年份</option>
	                                <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                <%@ include file="inc/years.inc" %>
                               	</select>
								<p class="green-tip bottomTip" style="visibility:hidden">请填写小学，会找到小学同学</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
							</td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
						</tr>
						<tr class="btn-box">
							<td></td>
							<td><input type="submit" value="保存我的个人信息" class="input-submit" /></td>
							<td></td>
						</tr>
					</table>
					</form>
					<!-- End:我上中学 -->
					
					<!-- 我工作了 -->
					<form  id="work_form" method="post" action="fidarks-bl?f=l" style="display:none;">
					<input type="hidden" value="30" name="stage"/>
					<table>
						<tr class="candyPaper">
							<td class="td-name"><em>*</em>我的单位 : </td>
							<td>
								<div id="" class="company-tip-box">
									<input type="text" class="input-text long" id="work" autocomplete="off" name="companyname" schema="companyName" />
									<input type="button" style="display:none;" value="找到同事" id="btn_find" class="parter-btn">
								</div>
								<p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->	
							</td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
						</tr>
						<tr class="candyPaper">					
							<td class="td-name"><em>*</em>毕业学校 :</td>
							<td class="schoolbox">
								<p>
									<label><input type="radio" bind="universityname" name="schooltype" id="swork-univ" schema="schoolType" />大学</label>
									<label><input type="radio" bind="highschoolname" name="schooltype" id="swork-high" schema="schoolType" />高中</label>
									<label><input type="radio" bind="techschoolname" name="schooltype" id="swork-tech" schema="schoolType" />中专</label>
									<label><input type="radio" bind="juniorhighschoolname" name="schooltype" id="swork-junior" schema="schoolType" />初中</label>
									<label><input type="radio" bind="elementaryschoolname" name="schooltype" id="swork-entary" schema="schoolType" />小学</label>
								</p>
								<p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
							</td>
							<td></td>
						</tr>
						<tr class="candyPaper" style="display:none;">					
							<td class="td-name"><em>*</em>大学:</td>
							<td class="schoolbox">
								<p>
									<input type="text" name="universityname" id="work_univ" class="mid input-text  s-select s-univ" schema="univName" />
									<input type="hidden" name="universitycode" value="0" id="work_univ_id" />

									<select class="select select-gray" id="work_univ_year" name="universityyear" schema="univYear"><option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                	</select>
								</p>
								<p>
									<select name="universitydepartment" class="mid select select-gray univ_dept_select" id="work_univ_dept" schema="univDepart">
										<option value="">请选择院系</option>
									</select>
									<select id="work_univ_type" name="coursetype" class="short select " schema="univCourse">
										<option value="">请选择类型</option>
										<option value="大学生">大学生</option>
										<option value="硕士">硕士</option>
										<option value="博士">博士</option>
										<option value="其他">其他</option>
									</select>
								</p>
								<p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
							</td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
						</tr>
						<tr class="candyPaper" id="swork_mid_high_tr" style="display:none;">
							<td class="td-name">
	                            <em>*</em>高中 : 
	                        </td>
	                        <td>
                                <input type="text" id="work_mid_high" name="highschoolname" class="mid input-text s-select s-high" schema="highName"/>
                                <input type="hidden" id="work_mid_high_id" name="highschoolcode"/>
                                <select id="work_mid_high_year" name="highschoolyear" class="select select-gray" schema="highYear"><option value="">入学年份</option>
                                	<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                <%@ include file="inc/years.inc"%>
                                </select>
		                        <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
	                        </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>       
                        
                        <tr class="candyPaper" id="swork_mid_tech_tr" style="display:none;">
                            <td class="td-name"><em>*</em>中专: </td>
                            <td>
                                <!-- <input type="hidden" value="10" name="stage" /> -->
                                <input type="text" class="mid input-text s-select s-tech" name="techschoolname" id="work_mid_tech" schema="techName"/> 
                                <input type="hidden" id="work_mid_tech_id" value="0" name="techschoolcode"/>

                                <select class="select select-gray"  name="techschoolyear"  id="work_mid_tech_year" schema="techYear">
                                        <option value="" >入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
								</select>
                                <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>
                        
						<tr class="candyPaper" id="swork_mid_junior_tr" style="display:none;">
	                        <td class="td-name"><em>*</em>初中 : </td>
	                        <td>
	                            <input type="text" class="mid input-text s-select s-junior" name="juniorhighschoolname" id="work_mid_junior" schema="juniorName"/>
	                            <input type="hidden" id="work_mid_junior_id" name="juniorhighschoolcode" value="0"/>
	                            <select class="select select-gray" name="juniorhighschoolyear" id="work_mid_junior_year" schema="juniorYear">
	                                <option value="">入学年份</option>
	                                <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                <%@ include file="inc/years.inc" %>
	                            </select>
	                            <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>

                        <tr class="candyPaper" id="swork_mid_entary_tr" style="display:none;">
                            <td class="td-name"><em>*</em>小学 : </td>
                            <td>
                                <input type="text" class="mid input-text" id="work_mid_entary" autocomplete="off" name="elementaryschoolname" schema="entaryName"/>
                                <input  type="hidden" id="work_mid_entary_id" name="elementaryschoolcode"/>
                                 <select class="select select-gray" id="work_mid_entary_year" name="elementaryschoolyear" schema="entaryYear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                 </select>
                                 <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>

						<tr class="candyPaper">
							<td class="td-name"><em>*</em>当前所在地 : </td>
							<td>
								<select class="select cur-plc short" id="currentprovince" name="currentprovince" schema="currProvince">
									<option value="">省份</option>
								</select>
								<select class="select mid select-short" id="currentprovince_city" schema="currCity">
									<option value="">城市</option>
								</select>
								<span style="display:none;" id="currentprovince_mes" class="form-error tips"><span>请填写所在地</span></span>
								<p class="green-tip bottomTip" style="visibility:hidden">提示文案请用visibility:visible;显示</p>
								<input type="hidden" id="currentprovince_city_code" name="currentcitycode" />
                            	<input type="hidden" id="currentprovince_city_name" name="currentcityname" />
							</td>
							<td></td>
						</tr>
						<tr class="btn-box">
							<td></td>
							<td><input type="submit" value="保存我的个人信息" class="input-submit" /></td>
							<td></td>
						</tr>
					</table>
					</form>
					<!-- End:我工作了 -->
					
					<!-- 其他阶段 -->
					<form  id="none_form" method="post" action="fidarks-qt?f=l" style="display:none;" >
					<input type="hidden" value="90" name="stage"/>
			        <table>
                        <tr class="candyPaper" id="none_cur_tr">
                            <td class="td-name"><em>*</em>当前所在地 : </td>
                            <td>
                            	<select class="select cur-plc select-gray" id="none_currentprovince" name="currentprovince"  style="width:80px;" schema="currProvince">
	                                <option value="">省份</option>
	                            </select>
	                            <select class="select select-short" id="none_currentprovince_city">
	                                <option value="">城市</option>
	                            </select>
	                            <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
	                            <input type="hidden" name="currentcitycode"  id="none_currentprovince_city_code"/>
	                            <input type="hidden" name="currentcityname" id="none_currentprovince_city_name"  schema="currCity"/>
                            </td>
							<td></td>
                        </tr>
                        <tr class="candyPaper" id="none_home_tr">
                            <td class="td-name"><em>*</em>家乡 : </td>
                            <td>
	       	                    <select class="select cur-home select-gray" id="none_homeprovince" name="homeprovince"  style="width:80px;" schema="homeProvince">
	                                <option value="">省份</option>
	                            </select>
	                            <select class="select select-short"  id="none_homeprovince_city">
	                                <option value="">城市</option>
	                            </select>
	                            <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
	                            <input type="hidden" name="homecitycode"  id="none_homeprovince_city_code"/>
	                            <input type="hidden" name="homecityname"  id="none_homeprovince_city_name" schema="homeCity"/>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>
                        <tr class="candyPaper" id="none_schooltype">
                            <td class="td-name"><em>*</em>毕业学校 :</td>
                            <td class="schoolbox" >
                                <p>
									<label><input type="radio" bind="universityname" name="schooltype" id="swork-univ" schema="schoolType"/>大学</label>
									<label><input type="radio" bind="highschoolname" name="schooltype" id="swork-high" schema="schoolType" />高中</label>
									<label><input type="radio" bind="techschoolname" name="schooltype" id="swork-tech" schema="schoolType" />中专</label>
									<label><input type="radio" bind="juniorhighschoolname" name="schooltype" id="swork-junior" schema="schoolType" />初中</label>
									<label><input type="radio" bind="elementaryschoolname" name="schooltype" id="swork-entary" schema="schoolType" />小学</label>
								</p>
                                <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td></td>
                        </tr>
                        <tr class="candyPaper" id="none_univ_tr" style="display:none;" >
                            <td class="td-name"><em>*</em>大学 : </td>
                            <td>
                                <p>
									<input type="text" name="universityname" id="other_univ" class="mid input-text  s-select s-univ" schema="univName"/>
									<input type="hidden" name="universitycode" value="0" id="other_univ_id" />

									<select class="select select-gray" id="other_univ_year" name="universityyear" schema="univYear"><option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                	</select>
								</p>
								<p>
									<select name="universitydepartment" class="mid select select-gray univ_dept_select" id="other_univ_dept" schmea="univDepart">
										<option value="">请选择院系</option>
									</select>
									<select id="work_univ_type" name="coursetype" class="short select " schema="univCourse">
										<option value="">请选择类型</option>
										<option value="大学生">大学生</option>
										<option value="硕士">硕士</option>
										<option value="博士">博士</option>
										<option value="其他">其他</option>
									</select>
								</p>
								<p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
								<!-- 另外一个提示信息不要忘记了哈<p class="yellow-tip">填写单位信息，右侧会出现同事新头像</p>--><!--请用visibility:hidden;将提示信息隐藏，才不会页面跳动-->
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>

                        <tr class="candyPaper" id="none_mid_high_tr" style="display:none;" >
                            <td class="td-name" valign="top"><em>*</em>高中 : </td>
                            <td>
	                            <input type="text" id="none_mid_high" name="highschoolname" class="mid input-text s-select s-high" schema="highName"/>
	                            <input type="hidden" id="none_mid_high_id" name="highschoolcode"/>
	                            <select id="none_mid_high_year" name="highschoolyear" class="select select-gray" schema="highYear"><option value="">入学年份</option>
	                            <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                            <c:set var="strutsOptionYearComponent_end" value="1900" />
	                            <%@ include file="inc/years.inc"%>
	                            </select>
	                            <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>       
                        
						<tr class="candyPaper" id="none_mid_tech_tr" style="display:none;" >
                            <td class="td-name"><em>*</em>中专: </td>
                            <td>
                                <input type="text" class="mid input-text s-select s-tech" name="techschoolname" id="none_mid_tech" schema="techName"/> 
                                <input type="hidden" id="none_mid_tech_id" value="0" name="techschoolcode"/>

                                <select class="select select-gray"  name="techschoolyear"  id="none_mid_tech_year" schema="techYear">
                                        <option value="" >入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
								</select>
                                <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>

                        <tr class="candyPaper" id="none_mid_junior_tr" style="display:none;" >
                            <td class="td-name"><em>*</em>初中 : </td>
                            <td>
								<input type="text" class="mid input-text s-select s-junior" name="juniorhighschoolname" id="none_mid_junior" schema="juniorName"/>
	                            <input type="hidden" id="none_mid_junior_id" name="juniorhighschoolcode" value="0"/>
	                            <select class="select select-gray" name="juniorhighschoolyear" id="none_mid_junior_year" schema="juniorYear">
	                                <option value="">入学年份</option>
	                            	<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                <%@ include file="inc/years.inc" %>
	                            </select>
	                            <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>
                        
                        
                        <tr class="candyPaper" id="none_mid_entary_tr" style="display:none;" >
                            <td class="td-name"><em>*</em>小学 : </td>
                            <td>
                                <input type="text" class="mid input-text" id="none_mid_entary" autocomplete="off" name="elementaryschoolname" schema="entaryName"/>
                                <input  type="hidden" id="none_mid_entary_id" name="elementaryschoolcode"/>
                                 <select class="select select-gray" id="none_mid_entary_year" name="elementaryschoolyear" schema="entaryYear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                 </select>
                                 <p class="green-tip bottomTip" style="visible:hidden;">填写单位信息，右侧会出现同事新头像</p>
                            </td>
							<td><a class="arrow-btn" href="#nogo"></a></td>
                        </tr>
                        <tr class="btn-box">
	                        <td></td>
	                        <td>
	                            <input type="submit" value="保存我的个人信息" class="input-submit" />
	                            <div class="register-tips">
	                                <div class="arrow"></div>
	                            </div>
	                        </td>
                        </tr>
                    </table>	
            		</form>
					<!-- End:其他阶段 -->
				</div>
				<!-- End:资料 -->
				<div class="side-mod" id="findThem" style="display:none;"></div>
				<div id="Introduction" class="side-mod">
					<img src="http://a.xnimg.cn/imgpro/reg/reg-guide/find-fri.png" alt="" />
				</div>
			</div>
			<div class="b_text">
			</div>
		</div>

	</div>
	<script type="text/javascript">
		XN.dom.ready(function(){
			FillInfo.init(Guide_reg_stage);
			var u_type = '${lead_type_flag}';
			if('_ABD'==u_type){
				$('univ_year').value=2011;
				$('univ_high_year').value=2008;
			}

		});
		var page_id = "p11";  //统计用的页面id
	</script>
	<%@ include file="/inc/guide_stat.inc" %>
	<!-- 组件 -->
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.currentPlace.js?ver=$revxxx$"></script>
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>
</body>
</html>
