<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
	<link href="${applicationScope.urlSmallStatic}/favicon-${domainIdentified["siteNameAbbr"]}.ico?ver=2" rel="shortcut icon" type="image/x-icon" />
	<link href="${applicationScope.urlSmallStatic}/renren_logo_apple.png" rel="apple-touch-icon" />
	<link href="${applicationScope.urlSmallStatic}/csspro/base/layout.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
	<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/base.js?ver=$revxxx$"></script>    
    <script type="text/javascript">
    XN.getFileVersion(
    [
        '${applicationScope.urlSmallStatic}/csspro/apps/profile.css?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.app.recommendFriend.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.ui.pager.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.ui.multiFriendSelectorBox.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/csspro/module/friendSelector.css?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/editor/tiny_mce.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/allunivlist.js?ver=$revxxx$'
    ]);    
    </script>
	<%@ include file="/inc/patch.inc" %>
	<link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.4.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<title>${domainIdentified["siteName_fillinfo"]} - 选择你认识的人加为好友</title>
</head>
<body id="home" class="guide Infor">
<%--menu-bar begin--%>
<%@ include file="/inc/header-guide-flow.inc" %>
<%--menu-bar end--%><%--header-information-guide.inc end--%>


<c:set var="stage" value="${formtype}" ></c:set>
<c:forEach var="item" items="${infolist}">
<div style="display:none" class='hidden_reginfo'>{'${item.type}':'${item.displayname}'}</div>
</c:forEach>
<div id="content">
	<div class="user-data clearfix">
		<div class="title" id="infor-title">
			<h1>填信息，找朋友</h1>
		</div><%--title end--%>
    <div class="nav-tab" id="nav-tab" style="display:none">
                <h2 class="clearfix" id="nav-tabs" style="display:none">
                   <%-- <a style="width: 30px;" href="#nogo" class="selector">全部</a>
                    <a href="#nogo">家里蹲大学蹲大学</a>
                    <a href="#nogo">家里蹲大学蹲大学</a>
                    <a href="#nogo">家里蹲大学蹲大学</a>
                    <a style="width: 30px;" class="other" href="#nogo">其他</a>--%>
                </h2>
                <ul class="other-infor" id="nav-other" style="display:none">
                    <%--<li><a href="#">其他大学</a></li>
                    <li class="other-sele"><a href="#">其他大学大学大学</a></li>
                    <li><a href="#">其他大学大学</a></li>
                    <li><a href="#">其他大学大学</a></li>
                    <li><a href="#">其他大学</a></li>
                    <li><a href="#">其他大学大学</a></li>--%>
                </ul>
                <div class="friend-choose" id="friend-choose"></div>
            </div>
		<%--friend-choose end--%>
		<div class="friend-box" id="friend-box">
			<h1>根据个人信息找到你的好友</h1>
			<ul class="clearfix">
				<li>
					<p><img src="${applicationScope.urlStatic}/imgpro/avatar/d_default_50.gif" width="50" height="50"/></p>
					<span>老同学</span>
					<span>加为好友</span>
				</li>
				<li>
					<p><img src="${applicationScope.urlStatic}/imgpro/avatar/d_default_girl_50.gif" width="50" height="50"/></p>
					<span>老同学</span>

					<span>加为好友</span>
				</li>
				<li>
					<p><img src="${applicationScope.urlStatic}/imgpro/avatar/d_default_50.gif" width="50" height="50"/></p>
					<span>老同学</span>
					<span>加为好友</span>
				</li>

			</ul>
		</div><%--friend-box end--%>
		<div class="fill-data">
			<div class="fill-data-box">
				<div class="main-data secondary">
					<form id="addStage" action="<%=request.getAttribute("saveaction") %>">
                        <input type="hidden" name="stage" value="${stage}" />
						<table>
                            <%--其它身份必填项--%>
							<c:if test="${stage==90}">
							<tr id="cur-plc">
                            <td class="td-name">
                                <em>*</em>当前所在地 : 
                            </td>
                            <td>
                                <select class="select cur-plc select-gray" id="currentprovince" name="currentprovince" style="width:80px;">
                                    <option value="">省份</option>
                                </select>
                                <select class="select select-gray select-city" id="currentprovince_city">
                                    <option value="">城市</option>
                                </select>
                                <input type="hidden" id="currentprovince_city_code" name="currentcitycode" /><input type="hidden" id="currentprovince_city_name" name="currentcityname" />
                            </td>
							</tr>
                            <tr id="cur-home">
                                <td class="td-name">
                                    <em>*</em>家乡 : 
                                </td>
                                <td>
                                    <select class="select cur-home select-gray" name="homeprovince" id="homeprovince" style="width:80px;">
                                        <option value="">省份</option>
                                    </select>
                                    <select class="select select-gray select-city" id="homeprovince_city">
                                        <option value="">城市</option>
                                    </select>
                                    <input type="hidden" name="homecitycode" id="homeprovince_city_code"/><input type="hidden" name="homecityname" id="homeprovince_city_name"/>
                                </td>
                            </tr>
							</c:if>

                           <%--已经工作必填项--%>
                            <c:if test="${stage==30}">
                                <tr id="work_tr">
                                    <td class="td-name">
                                        <em>*</em>单位 : 
                                    </td>
                                    <td>
                                        <input type="text" class="input-text" id="work" name="companyname" />
                                    </td>
                                    <tr>
                                        <td></td><td style="color:#888">填写信息能够帮你找到好友&nbsp;不会对陌生人公开:)</td>
                                    </tr>
                                </tr>
                            </c:if>

					    	<%--已经工作和其它都会有下面这个学校选择--%>
                           <c:if test="${stage==30 || stage==90}">                              
                                <tr id="s_schooltype">
                                    <td class="td-name"><em>*</em>选择学校:</td>
                                    <td>
                                        <label for="s-univ">
                                            <input type="radio" id="s-univ" name="schooltype"/>大学
                                        </label>
                                        <label for="s-high">
                                            <input type="radio" id="s-high" name="schooltype"/>高中
                                        </label>
                                        <label for="s-junior">
                                            <input type="radio" id="s-junior" name="schooltype"/>初中
                                        </label>
                                        <label for="s-entary">
                                             <input type="radio" id="s-entary" name="schooltype"/>小学
                                        </label>
                                    </td>
                                </tr>
                                <tr id="univ_tr" style="display:none">
                                    <td class="td-name">
                                        <em>*</em>大学 : 
                                    </td>
                                    <td>
                                        <input type="text" class="input-text  s-select s-univ" id="univ" name="universityname" />
                                        <input type="hidden" id="univ_id" value="0" name="universitycode" />
                                        <select class="select select-gray" id="univ_year" name="universityyear">
                                            <option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc" %>
                                        </select>
                                    </td>
                                </tr>
                                <tr id="univ_dept_type_tr" style="display:none">
                                    <td class="td-name">
                                    </td>
                                    <td>
                                        <select id="univ_dept" class="select select-gray univ_dept_select" name="universitydepartment">
                                            <option value="">选择院系</option>
                                        </select>
                                        <select class="select" name="coursetype" id="univ_type">
                                            <option value="">请选择类型</option>
                                            <option value="大学生" selected>本科生</option>
                                            <option value="硕士">硕士</option>
                                            <option value="博士">博士</option>
                                        </select>
                                    </td>
                                </tr>
                                <tr id="mid_high_tr" style="display:none">
                                    <td class="td-name" valign="top">
                                        <em>*</em>高中 : 
                                    </td>
                                    <td>
                                    <table>
                                        <tbody>
                                            <tr id="mid_high_true_tr">
                                                <td>
                                                    <input type="text" id="mid_high" name="highschoolname" class="input-text s-select s-high"/>
                                                    <input type="hidden" id="mid_high_id" name="highschoolcode"/>
                                                    <select id="mid_high_year" name="highschoolyear" class="select select-gray">
                                                        <option value="">入学年份</option>
                                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                                        <%@ include file="inc/years.inc" %>
                                                    </select>
                                                </td>
                                            </tr>
                                            <%--
                                            <tr>
                                                <td>
                                                      高一 <input id="grade1" name="highschoolclass1" type="text" class="input-text digital"/>班
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                        高二 <input id="grade2" name="highschoolclass2" type="text" class="input-text digital"/>班
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                      高三 <input id="grade3" name="highschoolclass3" type="text" class="input-text digital"/>班
                                                </td>
                                            </tr>
                                            --%>
                                        </tbody>
                                    </table>
                                    </td>
                                </tr>		
                                <tr id="mid_junior_tr" style="display:none">
                                    <td class="td-name">
                                        <em>*</em>初中 : 
                                    </td>
                                    <td>
                                        <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_junior"/>
                                        <input type="hidden" id="mid_junior_id" name="juniorhighschoolcode" value="0"/>
                                        <select class="select select-gray" name="juniorhighschoolyear" id="mid_junior_year">
                                            <option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc" %>
                                        </select>
                                    </td>
                                </tr>
                                <tr id="mid_entary_tr" style="display:none">
                                    <td class="td-name">
                                        <em>*</em>小学 : 
                                    </td>
                                    <td>
                                        <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
                                        <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
                                        <select class="select select-gray" id="mid_entary_year" name="elementaryschoolyear">
                                            <option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc" %>
                                        </select>
                                    </td>
                                </tr>
                           </c:if>


                            <%--大学必填项--%>
                            <c:if test="${stage==20}">
                                <tr id="univ_tr">
                                    <td class="td-name">
                                        <em>*</em>大学 : 
                                    </td>
                                    <td>
                                        <input type="text" class="input-text  s-select s-univ" id="univ" name="universityname" />
                                        <input type="hidden" id="univ_id" value="0" name="universitycode" />
                                        <select class="select select-gray" id="univ_year" name="universityyear">
                                            <option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc" %>
                                        </select>
                                    </td>
                                </tr>
                                <tr id="univ_dept_type_tr">
                                    <td class="td-name">
                                    </td>
                                    <td>
                                        <select id="univ_dept" class="select select-gray univ_dept_select" name="universitydepartment">
                                            <option value="">选择院系</option>
                                        </select>
                                        <select class="select select-gray" name="coursetype" id="univ_type">
                                            <option value="">请选择类型</option>
                                            <option value="大学生">本科生</option>
                                            <option value="硕士">硕士</option>
                                            <option value="博士">博士</option>
                                        </select>
                                    </td>
                                </tr>
                                <tr id="mid_high_tr">
                                    <td class="td-name" valign="top">
                                        <em>*</em>高中 : 
                                    </td>
                                    <td>
                                    <table>
                                        <tbody>
                                            <tr id="mid_high_true_tr">
                                                <td>
                                                    <input type="text" id="mid_high" name="highschoolname" class="input-text s-select s-high"/>
                                                    <input type="hidden" id="mid_high_id" name="highschoolcode"/>
                                                    <select id="mid_high_year" name="highschoolyear" class="select select-gray">
                                                        <option value="">入学年份</option>
                                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                                        <%@ include file="inc/years.inc" %>
                                                    </select>
                                                </td>
                                            </tr>
                                            <%--<tr>
                                                <td>
                                                      高一 <input id="grade1" name="highschoolclass1" type="text" class="input-text digital"/>班
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                        高二 <input id="grade2" name="highschoolclass2" type="text" class="input-text digital"/>班
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                      高三 <input id="grade3" name="highschoolclass3" type="text" class="input-text digital"/>班
                                                </td>
                                            </tr>--%>
                                        </tbody>
                                    </table>
                                    </td>
                                </tr>		
							</c:if>
						
                            <%--高中生必填项--%>
                            <c:if test="${stage==12}">
                            <tr id="mid_high_tr">
                                <td class="td-name" valign="top">
                                    <em>*</em>高中 : 
                                </td>
                                <td>
                                <table>
                                    <tbody>
                                        <tr id="mid_high_true_tr">
                                            <td>
                                                <input type="text" id="mid_high" name="highschoolname" class="input-text s-select s-high"/><input type="hidden" id="mid_high_id" name="highschoolcode"/>
                                                <select id="mid_high_year" name="highschoolyear" class="select select-gray">
                                                    <option value="">入学年份</option>
													<c:set var="strutsOptionYearComponent_begin" value="2011" />
                                                    <c:set var="strutsOptionYearComponent_end" value="1900" />
                                                    <%@ include file="inc/years.inc" %>
                                                </select>
                                            </td>
                                        </tr>
                                        <tr id="mid_high_grades">
                                            <td>
                                             	  高一 <input id="grade1" name="highschoolclass1" type="text" class="input-text digital"/>班
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                            	    高二 <input id="grade2" name="highschoolclass2" type="text" class="input-text digital"/>班
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                              	  高三 <input id="grade3" name="highschoolclass3" type="text" class="input-text digital"/>班
												  <%--<span class="form-error-box"><span class="form-error"><span>请填写中专技校</span></span></span>--%>
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                                </td>
                            </tr>		
                            <tr id="mid_junior_tr">
                                <td class="td-name">
                                    <em>*</em>初中 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_junior"/>
                                    <input type="hidden" id="mid_junior_id" name="juniorhighschoolcode" value="0"/>
                                    <select class="select select-gray" name="juniorhighschoolyear" id="mid_junior_year">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                            </c:if>
						
                            <%--中专技校必填项--%>
                            <c:if test="${stage==13}">
                                <tr id="mid_tech_tr">
                                    <td class="td-name">
                                        <em>*</em>中专技校 : 
                                    </td>
                                    <td>
                                        <input type="hidden" value="-1" name="schooltype" id="mid_sh_type"/>
                                        <input type="text" class="input-text s-select s-tech" name="techschoolname" id="mid_tech"/>
                                        <input type="hidden" id="mid_tech_id" value="0" name="techschoolcode"/>
                                        <select class="select select-gray" name="techschoolyear" id="mid_tech_year">
                                            <option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc" %>
                                        </select>
                                    </td>
                                </tr>
                                <tr id="mid_junior_tr">
                                    <td class="td-name">
                                        <em>*</em>初中 : 
                                    </td>
                                    <td>
                                        <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_junior"/>
                                        <input type="hidden" id="mid_junior_id" name="juniorhighschoolcode" value="0"/>
                                        <select class="select select-gray" name="juniorhighschoolyear" id="mid_junior_year">
                                            <option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc" %>
                                        </select>
                                    </td>
                                </tr>
                            </c:if>
						    
                            <%--初中生必填项--%>	
                            <c:if test="${stage==11}">
                            <tr id="mid_junior_tr">
                                <td class="td-name">
                                    <em>*</em>初中 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_junior"/>
                                    <input type="hidden" id="mid_junior_id" name="juniorhighschoolcode" value="0"/>
                                    <select class="select select-gray" name="juniorhighschoolyear" id="mid_junior_year">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                            <tr id="mid_entary_tr">
                                <td class="td-name">
                                    <em>*</em>小学 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
                                    <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
                                    <select class="select select-gray" id="mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
							</c:if>	
							
                            <%--初中和工作时不显示--%>
                            <c:if test="${stage!=11 and stage!=30 }">
                            <tr>
                                <td></td>
                                <td><a id="addDetail" href="#nogo" onclick="$('fillMore').show();this.style.display='none';guide_add_stats('p_fillinfo_15_m_info_a_moreinfo')">+添加更多资料</a></td>
                            </tr>
                            </c:if>
						</table>

						<%--下面的是已经有身份的人要选择的东西--%>
						<table id="fillMore" style="display:none">
                       
                        <%--大学生选填的资料--%>
                        <c:if test="${stage==20}">
                            <tr id="mid_junior_tr">
                                <td class="td-name">
                                    初中 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_junior"/>
                                    <input type="hidden" id="mid_junior_id" name="juniorhighschoolcode" value="0"/>
                                    <select class="select select-gray" name="juniorhighschoolyear" id="mid_junior_year">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                            <tr id="mid_entary_tr">
                                <td class="td-name">
                                    小学 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
                                    <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
                                    <select class="select select-gray" id="mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                            <tr id="work_tr">
                                <td class="td-name">
                           		         单位 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="work" name="companyname" />
                                </td>
                            </tr>
                        </c:if>

                        <%--高中生选填的项--%>
                        <c:if test="${stage==12}">
                            <tr id="mid_entary_tr">
                                <td class="td-name">
                                    小学 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
                                    <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
                                    <select class="select select-gray" id="mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                        </c:if>

                        <%--中专技校选填项--%>
                        <c:if test="${stage==13}">
                            <tr id="mid_entary_tr">
                                <td class="td-name">
                                    小学 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
                                    <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
                                    <select class="select select-gray" id="mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                        </c:if>

                        <%--其它和工作情况下的选填项--%>
                        <%--<c:if test="${stage==30 || stage==90}">
                            <tr id="univ_tr">
                                <td class="td-name">
                                    大学 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text  s-select s-univ" id="univ" name="universityname" />
                                    <input type="hidden" id="univ_id" value="0" name="universitycode" />
                                    <select class="select select-gray" id="univ_year" name="universityyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                            <tr id="univ_dept_type_tr">
                                <td class="td-name">
                                </td>
                                <td>
                                    <select id="univ_dept" class="select select-gray univ_dept_select" name="universitydepartment">
                                        <option value="">选择院系</option>
                                    </select>
                                    <select class="select select-gray" name="coursetype" id="univ_type">
                                        <option value="">请选择类型</option>
                                        <option value="大学生">本科生</option>
                                        <option value="硕士">硕士</option>
                                        <option value="博士">博士</option>
                                    </select>
                                </td>
                            </tr>
                            <tr id="mid_high_tr">
                                <td class="td-name" valign="top">
                                    高中 : 
                                </td>
                                <td>
                                <table>
                                    <tbody>
                                        <tr id="mid_high_true_tr">
                                            <td>
                                                <input type="text" id="mid_high" name="highschoolname" class="input-text s-select s-high"/>
                                                <input type="hidden" id="mid_high_id" name="highschoolcode"/>
                                                <select id="mid_high_year" name="highschoolyear" class="select select-gray">
                                                    <option value="">入学年份</option>
                                                    <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                                    <c:set var="strutsOptionYearComponent_end" value="1900" />
                                                    <%@ include file="inc/years.inc" %>
                                                </select>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                  高一 <input id="grade1" name="highschoolclass1" type="text" class="input-text digital"/>班
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                    高二 <input id="grade2" name="highschoolclass2" type="text" class="input-text digital"/>班
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                  高三 <input id="grade3" name="highschoolclass3" type="text" class="input-text digital"/>班
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                                </td>
                            </tr>		
                            <tr id="mid_junior_tr">
                                <td class="td-name">
                                    初中 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="mid_junior"/>
                                    <input type="hidden" id="mid_junior_id" name="juniorhighschoolcode" value="0"/>
                                    <select class="select select-gray" name="juniorhighschoolyear" id="mid_junior_year">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                            <tr id="mid_entary_tr">
                                <td class="td-name">
                                    小学 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="mid_entary" name="elementaryschoolname"/>
                                    <input type="hidden" id="mid_entary_id" name="elementaryschoolcode"/>
                                    <select class="select select-gray" id="mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="2011" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                    </select>
                                </td>
                            </tr>
                        </c:if>
                        --%>
                        <c:if test="${stage == 90}">
                            <tr id="work_tr">
                                <td class="td-name">
                                    单位 : 
                                </td>
                                <td>
                                    <input type="text" class="input-text" id="work" name="companyname" />
                                </td>
                            </tr>
                        </c:if>
						</table>
					</form>
				</div><%--main-data secondary end--%>
				<div class="main-data">
					<div class="save-box">
						<input type="button"  onfocus="this.blur()" value="保存并查找" class="input-submit" id="saveAndSearch"/> 
						<%--<input type="button" value="保存并查找" class="input-submit gray" />--%>
					</div>
				</div><%--main-data end--%>
			</div><%--fill-data-box end--%>

		</div><%--fill-data end--%>
		<div class="btn-box" id="btn-box" style="display:none">
            <form action="pvcoral-s" id="next-step-form">
                <div style="display:none" id="friends-count-filedset"> </div>
                <input type="hidden" name="stage" value="${stage}" />
                <input type="button" value="下一步" class="input-next gray" onfocus="this.blur()" id="nextStep"/>
            </form>
	    </div>
	</div><%--user-data end--%>
</div><%--content end--%>
<%-- 组件 --%>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.currentPlace.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.stormAddStage-v2.2.js?ver=$revxxx$"></script>
<%-- 特定于页面的代码 --%>
<script type="text/javascript">
	<c:if test="${stage==11}"> 
		XN.page.guide.stormAddStage.initJuniorForm();
	</c:if>
	<c:if test="${stage==12}"> 
		XN.page.guide.stormAddStage.initHighForm();
	</c:if>
	<c:if test="${stage==13}">
		XN.page.guide.stormAddStage.initTechForm();
	</c:if>
	<c:if test="${stage==20}">
		XN.page.guide.stormAddStage.initUnivForm();
	</c:if>
	<c:if test="${stage==30}"> 
		XN.page.guide.stormAddStage.initWorkForm();
	</c:if>
	<c:if test="${stage==90}"> 
		XN.page.guide.stormAddStage.initOtherForm();
	</c:if>
	XN.page.guide.stormAddStage.initSaveSearch();

	XN.dom.ready(function(){
        <%--//新的统计--%>
        guideStats.send('rggp2v2a0');
        
        <%--//添加更多资料--%>
        XN.event.addEvent($('addDetail'),'click', function(){
            guideStats.send('rggp2v2a1')
        }); 
        
        <%--//保存并查找--%>
        XN.event.addEvent($('saveAndSearch'),'click', function(){
             guideStats.send('rggp2v2a2')
        }); 
	})
</script>
<%@ include file="/inc/footer-guide-flow.inc" %>
</body>
</html>
