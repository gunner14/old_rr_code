<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="http://s.xnimg.cn/csspro/old/base.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<link href="http://s.xnimg.cn/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css">
	<link href="http://s.xnimg.cn/css/friends.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<link href="http://s.xnimg.cn/css/guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	
	<title>${domainIdentified["siteName_fillinfo"]} - 完善个人信息</title>
</head>

<body>

<script type="text/javascript">
    new Ajax.Request('/xtrs?pma=p_reg_fillinfo_old_m_a_arrive', {
        method: 'get',
        parameters: '',
        onSuccess: function(transport){
        }
    });
</script>


<div id="container">
	<%@ include file="inc/menu_withif.inc" %>
	<div id="opi" class="page-wrapper clearfix">
		<div class="full-page-holder">
			<div class="full-page clearfix">
				<div class="compatible">
					<div id="content">
						<div class="new-guide">
							<h3 class="welcome">欢迎来到${domainIdentified["siteName"]}网，
							<%
							String name = (String)request.getAttribute("user_name");
							if(name !=null){
								out.println(name);
							}
							else{
								out.println("我的朋友");
							}
							%></h3>
							<%@ include file="inc/navbar_1fillinfo.inc" %>
							<div class="personal-info">
								<h4>填写你的个人信息</h4>
								<p class="description">
									简单的个人信息将帮助你快速找到你所认识的人。
								</p>
								<p id="err_panel" class="show-error" style="display:none">
									资料填写有误
								</p>
								<p id="err-univ-year" class="show-error" style="display:none">
									请选择你的大学入学年
								</p>
								<p id="err-high-year" class="show-error" style="display:none">
									请选择你的高中入学年
								</p>
								<p id="err-comp-name" class="show-error" style="display:none">
									请选择你的单位名称
								</p>
								<div class="pinfo-form">
										<p>
											<label>目前身份 : </label>
											<select id="identity" name="NONAME_001" class="select-long">
												<option value="-1" selected="selected">请选择您目前的身份</option>
												<option value="2">中学生</option>
												<option value="0">大学生</option>
												<option value="1">已经工作</option>
												<option value="3">以上都不是</option>
											</select>

										</p>
										

										<!-- university -->
										<div id="univ_panel" class="college" style="display:none">
										<html:form action="/fi-090729-dx?submit=1" styleId="univ_form">
											<p>
												<label for="universityname">
													大学 : 
												</label>
												<html:text styleId="univ"  property="universityname" value="请填写大学" styleClass="input-text s-select s-univ" />
												<html:hidden styleId="univ_id" property="universitycode" />
												<html:select styleId="univ_year" property="universityyear" styleClass="select-short">
													<html:option value="">大学入学年</html:option>
														<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
														<c:set var="strutsOptionYearComponent_end" value="1900" />
														<%@ include file="/inc/strutsOptionYearComponent.inc" %>
												</html:select>
											</p>
											<p id="univ_dept_p">
												<label class="hidden">
													大学 : 
												</label>
												<html:select styleId="univ_dept" property="universitydepartment" styleClass="select-long">
													<html:option value="">请选择院系</html:option>
												</html:select>
											</p>
											<p id="univ_high_p">
												<label>
													高中 : 
												</label>
												<html:text styleId="univ_high" property="highschoolname" styleClass="input-text s-select s-high" value="请填写高中" />
												<html:hidden styleId="univ_high_id" property="highschoolcode" />
												<html:select styleId="univ_high_year" property="highschoolyear" styleClass="select-short">
													<html:option value="">高中入学年</html:option>
													<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
													<c:set var="strutsOptionYearComponent_end" value="1900" />
													<%@ include file="/inc/strutsOptionYearComponent.inc"%>
												</html:select>
											</p>
											<p class="form-action">
												<input id="univ_submit" type="button" class="input-submit" value="保存并继续" />
											</p>
										</html:form>
										</div>
										
										
										






										<!-- middle school -->
										<div id="mid_panel" class="highschool" style="display:none">
										<html:form action="/fi-090729-zx?submit=1" styleId="mid_form">
											<p>
												<label class="hidden">
													高中 : 
												</label>
												<html:select styleId="mid_type" property="schooltype" styleClass="select-long">
													<html:option value="-1">选择学校类型</html:option>
													<html:option value="1">初中</html:option>
													<html:option value="0">高中</html:option>
													<html:option value="2">中专/技校</html:option>
												</html:select>
											</p>
											<p id="mid_high_p">
												<label>
													高中 : 
												</label>
												<html:text styleId="mid_high" property="highschoolname" styleClass="input-text s-select s-high" value="请填写高中" />
												<html:hidden styleId="mid_high_id" property="highschoolcode" />
												<html:select styleId="mid_high_year" property="highschoolyear" styleClass="select-short">
													<html:option value="">选择入学年</html:option>
													<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
													<c:set var="strutsOptionYearComponent_end" value="1900" />
													<%@ include file="/inc/strutsOptionYearComponent.inc" %>
												</html:select>
											</p>
											
											<ul id="mid_high_cl_p" >
												<li class="clearfix">
													<h3 class="banji">班级 :</h3>
													<label for="mid_high_cl_1">高一</label>
													<html:select styleId="mid_high_cl_1" property="highschoolclass1" title="${fZX.highschoolclass1}">
														<%@ include file="inc/highschoolclasslist.inc" %>
													</html:select>
												</li>
												
												<li class="clearfix">
													<label  for="mid_high_cl_2">高二</label>
													<html:select styleId="mid_high_cl_2" property="highschoolclass2" title="${fZX.highschoolclass2}">
														<%@ include file="inc/highschoolclasslist.inc" %>
													</html:select>
												</li>
												
												<li class="clearfix">
													<label  for="mid_high_cl_3">高三</label>
													<html:select styleId="mid_high_cl_3" property="highschoolclass3" title="${fZX.highschoolclass3}">
														<%@ include file="inc/highschoolclasslist.inc" %>
													</html:select>
												</li>
											</ul>
											
											<p id="mid_tech_p">
												<label>
													中专/技校 : 
												</label>
												<html:text styleId="mid_tech" property="techschoolname" styleClass="input-text s-select s-tech" value="请填写中专/技校" />
												<html:hidden styleId="mid_tech_id" property="techschoolcode" />
												<html:select styleId="mid_tech_year" property="techschoolyear" styleClass="select-short">
													<html:option value="">选择入学年</html:option>
													<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
													<c:set var="strutsOptionYearComponent_end" value="1900" />
													<%@ include file="/inc/strutsOptionYearComponent.inc"%>
												</html:select>
											</p>
						


											<p id="mid_junior_p">
												<label>
													初中 : 
												</label>
												<html:text styleId="mid_junior" property="juniorhighschoolname" styleClass="input-text s-select s-junior" value="请填写初中" />
												<html:hidden styleId="mid_junior_id" property="juniorhighschoolcode" />
												<html:select styleId="mid_junior_year" property="juniorhighschoolyear" styleClass="select-short">
													<html:option value="">选择入学年</html:option>
													<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
													<c:set var="strutsOptionYearComponent_end" value="1900" />
													<%@ include file="/inc/strutsOptionYearComponent.inc"%>
												</html:select>
											</p>
											
											<p id="mid_entary_p">
												<label>
													小学 : 
												</label>
												<html:text styleId="mid_entary" property="elementaryschoolname" styleClass="input-text" value="请填写小学" />
												<html:hidden styleId="mid_entary_id" property="elementaryschoolcode" />
												<html:select styleId="mid_entary_year" property="elementaryschoolyear" styleClass="select-short">
													<html:option value="">选择入学年</html:option>
													<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
													<c:set var="strutsOptionYearComponent_end" value="1900" />
													<%@ include file="/inc/strutsOptionYearComponent.inc"%>
												</html:select>
											</p>
											

											<p class="form-action">
												<input id="mid_submit" type="button" class="input-submit" value="保存并继续" />
											</p>
										</html:form>
										</div>
										










										<!-- work -->
										<div id="work_panel" class="work" style="display:none">
										<html:form action="/fi-090729-bl?submit=1" styleId="work_form">
											<p>
												<label>
													单位 : 
												</label>
												<html:text styleId="work" property="companyname" styleClass="input-text" />
											</P>
											<p>
												<label>
													所在地 : 
												</label>
												<html:select styleId="work_cur"  property="homeprovince" styleClass="select-short cur-plc">
													<%@ include file="/pages/profile/inProvince.jsp"%>
												</html:select>
												<select id="work_cur_city" class="select-short">
													<option value="">城市</option>
												</select>
												<html:hidden property="homecitycode" styleId="work_cur_city_code" />
												<html:hidden property="homecityname" styleId="work_cur_city_name" />
												<select id="work_cur_city_state" class="select-short">
													<option value="">地区</option>
												</select>
												<!--
												<span class="description">浏览器位置 : <a href="#nogo">郑州市</a></span>
												-->
											</p>
											<p id="work_univ_p">
												<label>
													大学 : 
												</label>
												<html:text styleId="work_univ"  property="universityname" value="请填写大学" styleClass="input-text s-select s-univ" />
												<html:hidden styleId="work_univ_id" property="universitycode" />
												<html:select styleId="work_univ_year" property="universityyear" styleClass="select-short">
													<html:option value="">大学入学年</html:option>
														<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
														<c:set var="strutsOptionYearComponent_end" value="1900" />
														<%@ include file="/inc/strutsOptionYearComponent.inc" %>
												</html:select>
											</p>
											<p id="work_univ_dept_p">
												<label class="hidden">
													大学 : 
												</label>
												<html:select styleId="work_univ_dept" property="universitydepartment" styleClass="select-long">
													<html:option value="">请选择院系</html:option>
												</html:select>
											</p>
											<p class="form-action">
												<input id="work_submit" type="button" class="input-submit" value="保存并继续" />
											</p>
										</html:form>
										</div>














										<!-- none  -->
										<div id="none_panel" class="allnot" style="display:none">
										<html:form action="/fi-090729-qt?submit=1" styleId="none_form">
											<p>
												<label>
													所在地 : 
												</label>
												<html:select styleId="none_cur" property="homeprovince" styleClass="select-short cur-plc">
													<%@ include file="/pages/profile/inProvince.jsp"%>
												</html:select>
												<select id="none_cur_city" class="select-short">
													<option value="">城市</option>
												</select>
												<html:hidden property="homecitycode" styleId="none_cur_city_code" />
												<html:hidden property="homecityname" styleId="none_cur_city_name" />
												<select id="none_cur_city_state"  class="select-short">
													<option value="">地区</option>
												</select>
												<!--
												<span class="description">浏览器位置 : <a href="#nogo">郑州市</a></span>
												-->
											</p>
											<p id="none_univ_p">
												<label>
													大学 : 
												</label>
												<html:text styleId="none_univ"  property="universityname" value="请填写大学" styleClass="input-text s-select s-univ" />
												<html:hidden styleId="none_univ_id" property="universitycode" />
												<html:select styleId="none_univ_year" property="universityyear" styleClass="select-short">
													<html:option value="">大学入学年</html:option>
														<c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
														<c:set var="strutsOptionYearComponent_end" value="1900" />
														<%@ include file="/inc/strutsOptionYearComponent.inc" %>
												</html:select>
											</p>
											<p id="none_univ_dept_p">
												<label class="hidden">
													大学 : 
												</label>
												<html:select styleId="none_univ_dept" property="universitydepartment" styleClass="select-long">
													<html:option value="">请选择院系</html:option>
												</html:select>
											</p>
											<p class="form-action">
												<input id="none_submit" type="button" class="input-submit" value="保存并继续" />
											</p>
										</html:form>
										</div>
										
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
	<c:set var="hideFooterLink" value="true" />
    <c:set var="noWebPager" value="true" />
	<%@ include file="/inc/footer-noad.inc" %>
</div>






<!-- 好吧, 脚本就放在这里 -->
<!-- 组件 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.currentPlace.js?ver=$revxxx$"></script>
<script type="text/javascript" src="http://s.xnimg.cn/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>

<!-- 特定于页面的代码 -->
<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.addStage.js?ver=$revxxx$"></script>
<script type="text/javascript">
	XN.dom.ready(function(){
        <%--//guide统计--%>
        guideStats.send('rggp1v3a0');

        <%--//确定按钮统计--%>
        var btnList = XN.dom.getElementsByClassName('input-submit');
        XN.array.each(btnList, function(i,v){
            if( v.type == 'button' ) { 
                 XN.event.addEvent(v,'click', function(){
                     guideStats.send('rggp1v3a1')
                 }); 
             }
        });
	});
</script>
</body>
</html>
