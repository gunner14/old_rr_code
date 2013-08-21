<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<%@page import="com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType"%><html xmlns="http://www.w3.org/1999/xhtml">

<head><!-- _fi_darkblue_ -->
    <%@ include file="/inc/head-guide-flow.inc" %>
    <link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-register.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <title>${domainIdentified["siteName_fillinfo"]} - 寻找你认识的人</title>
</head>
<body id="home" class="guide">
<div id="dropmenuHolder" class="dropmenu-holder rr">
    <div id="choose-box" class="choose-box" style="display:none">
        <input id="show-junior" type="button" value="初中" class="input-submit" /><br/>
        <input id="show-high" type="button" value="高中" class="input-submit" /><br/>
        <input id="show-tech" type="button" value="中专技校" class="input-submit" />
    </div>
</div>

<!--menu-bar begin-->
<%@ include file="/inc/header-guide-flow.inc" %>
<!--menu-bar end--><!--header-information-guide.inc end-->
<div id="content">
    <%
    int brown0 = IGuideUserLeadType.XFiveBrown_0;
    %>
    <c:set var="user_type_brown0" value="<%=brown0%>" />
    <c:choose>
        <c:when test="${requestScope.lead_user_type == user_type_brown0}">
            <div class="nav-register second clearfix">
                <p>1.寻找朋友</p>
                <p class="online">2.完善资料，找到熟人</p>
                <p>3.上传头像，进入首页</p>
            </div>
        </c:when>
        <c:otherwise>
            <div class="nav-register first clearfix">
		        <p class="online">1.完善资料</p>
		        <p>2.找到好友</p>
		        <p>3.上传头像，进入首页</p>
		    </div>
        </c:otherwise>
    </c:choose>     
    

    <div class="user-data">
        <div class="title titleimg">
            <h1><strong>完善资料，让朋友快点找到我。</strong></h1>
        </div><!--title end-->
        
        <div class="identity-data">
            <ul class="clearfix" id="choose-identity">
                <li class="me-work">
                    <label for="me-work"><span><input type="radio" name="identity"  id="me-work"/>我工作了</span></label>
                </li>
                <li class="me-university">
                    <label for="me-university"><span><input name="identity"  type="radio" id="me-university"/>我在上大学</span></label>

                </li>   
                <li class="me-secondary"><input name="identity"  type="radio" id="me-secondary"/><label><span id="choose-mid-con">&nbsp;我在上中学</span></label>
                    <!-- 
                    <div id="choose-box" class="choose-box" style="display:none">
                        <input id="show-junior" type="button" value="初中" class="input-submit" /><br/>
                        <input id="show-high" type="button" value="高中" class="input-submit" /><br/>
                        <input id="show-tech" type="button" value="中专技校" class="input-submit" />
                    </div>
                     -->
                </li>
                <li class="me-other">
                    <label for="me-other"><span><input type="radio" name="identity"  id="me-other"/>其他身份</span></label>
                </li>
            </ul>

        </div><!--identity-data end-->
        <div id="mid_panel" class="fill-data secondary"  style="display:none">
            <form id="mid_form" method="post" action="fillinfos-zx">
            <div class="main-data">
            <div class="fill-data-arrow"></div>
                    <table>
                        <tr id="mid_tech_tr">
                            <td class="td-name"><em>*</em>中专技校 : </td>
                            <td> 
                                <input type="hidden" value="-1" name="formZhongxue.schooltype" id="mid_sh_type"/>
                                <input type="hidden" value="10" name="formZhongxue.stage" />
                                <input type="text" class="input-text s-select s-tech" name="formZhongxue.techschoolname" id="mid_tech"/> 
                                <input type="hidden" id="mid_tech_id" value="0" name="formZhongxue.techschoolcode"/>
                                <select class="select select-gray"  name="formZhongxue.techschoolyear"  id="mid_tech_year">
                                        <option value="" >入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="mid_tech_mes" class="form-error tips"><span>请填写中专技校，会找到中专技校同学</span></span>
                            </td>
                        </tr>
                        <tr id="mid_high_tr">
                            <td class="td-name" valign="top"><em>*</em>高中 : </td>
                            <td>
                                <table>
                                    <tr id="mid_high_sh_tr">
                                        <td>
                                            <input type="text" class="input-text s-select s-high" name="formZhongxue.highschoolname" id="mid_high" /> 
                                            <input type="hidden" name="formZhongxue.highschoolcode" id="mid_high_id" value="0"/>
                                            <select class="select select-gray" name="formZhongxue.highschoolyear" id="mid_high_year">
                                                <option value="">入学年份</option>
                                                <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                                <c:set var="strutsOptionYearComponent_end" value="1900" />
                                                <%@ include file="inc/years.inc" %>
                                            </select>
                                            <span style="display:none;" id="mid_high_mes" class="form-error tips"><span>请填写高中，会找到高中同学</span></span>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>高一 : <input type="text"  id="grade1"  class="input-text digital" name="formZhongxue.highschoolclass1"/> 班　高二 : <input type="text"  id="grade2"  class="input-text digital" name="formZhongxue.highschoolclass2" /> 班　高三 : <input type="text"  id="grade3"  class="input-text digital" name="formZhongxue.highschoolclass3" /> 班</td>
                                    </tr>
                                </table>
                            </td>
                        </tr>           
                        <tr id="mid_junior_tr">
                            <td class="td-name"><em>*</em>初中 : </td>
                            <td>
                                <input type="text" class="input-text s-select s-junior" name="formZhongxue.juniorhighschoolname" id="mid_junior"/>
                                <input type="hidden" id="mid_junior_id"  name="formZhongxue.juniorhighschoolcode" value="0"/>
                                <select class="select select-gray" name="formZhongxue.juniorhighschoolyear" id="mid_junior_year">
                                    <option value="">入学年份</option>
                                    <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                    <c:set var="strutsOptionYearComponent_end" value="1900" />
                                    <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="mid_junior_mes" class="form-error tips"><span>请填写初中，会找到初中同学</span></span>
                            </td>
                        </tr>

                        <tr id="mid_entary_tr">
                            <td class="td-name"><em>*</em>小学 : </td>
                            <td>
                                <input type="text" class="input-text" id="mid_entary" name="formZhongxue.elementaryschoolname"/>
                                <input  type="hidden" id="mid_entary_id" name="formZhongxue.elementaryschoolcode"/>
                                 <select class="select select-gray" id="mid_entary_year" name="formZhongxue.elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                 </select>
                                 <span  style="display:none;" id="mid_entary_mes" class="form-error tips"><span>请填写小学，会找到小学同学</span></span>
                            </td>
                        </tr>
                        <tr class="btn-box">
                            <td></td>
                            <td>
                                <input type="submit" value="保存并继续" class="input-submit" />
                                <div class="register-tips">
                                    <div class="arrow"></div>
    
                                    <p>这些信息会帮助你找到认识的人，<br/>绝不会对陌生人公开，请放心添加。</p>
                                </div>
                            </td>
                        </tr>
                    </table>
            </div><!--main-data end-->
            <!-- <div class="btn-box"><input type="submit" value="保存并继续" class="input-submit" /></div> -->
            </form>
        </div><!--fill-data end-->
        <div id="univ_panel" class="fill-data university"  style="display:none">
            <form  id="univ_form" method="post" action="fillinfos-dx">
            <div class="main-data">
            <div class="fill-data-arrow"></div>
                    <table>
                        <tr id="univ_tr">
                            <td class="td-name"><em>*</em>大学 : </td>
                            <td>
                                <input type="hidden" value="20" name="formDaxue.stage" />
                                <input type="text" class="input-text  s-select s-univ" id="univ" name="formDaxue.universityname" />
                                <input type="hidden" id="univ_id" value="0" name="formDaxue.universitycode" />
                                <select class="select select-gray" id="univ_year" name="formDaxue.universityyear" ><option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="univ_mes" class="form-error tips"><span>请填写大学，会找到大学同学</span></span>
                            </td>
                        </tr>
                        <tr id="univ_dept_type_tr">
                            <td class="td-name"></td>
                            <td>
                                <select id="univ_dept" class="select select-gray" name="formDaxue.universitydepartment">
                                    <option value="">选择院系</option>
                                </select>

                                <select class="select select-gray" name="formDaxue.coursetype" id="univ_type">
                                    <option value="">请选择类型</option>
                                    <option value="大学生">大学生</option>
                                    <option value="硕士">硕士</option>
                                    <option value="博士">博士</option>
                                </select>
                                
                            </td>
                        </tr>
                        <tr  id="univ_high_tr">
                            <td class="td-name"><em>*</em>高中 : </td>
                            <td>
                                <input type="text" class="input-text  s-select s-high" id="univ_high" name="formDaxue.highschoolname"/> 
                                <input type="hidden" id="univ_high_id" value="0" name="formDaxue.highschoolcode"/>
                                <select class="select select-gray"  name="formDaxue.highschoolyear" id="univ_high_year">
                                    <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc"%>
                                </select>
                                <span style="display:none;" id="univ_high_mes" class="form-error tips"><span>请填写高中，会找到高中同学</span></span>
                            </td>
                        </tr>
                        <tr class="btn-box">
                            <td></td>
                            <td>
                                <input type="submit" value="保存并继续" class="input-submit" />
                                <div class="register-tips">
                                    <div class="arrow"></div>
    
                                    <p>这些信息会帮助你找到认识的人，<br/>绝不会对陌生人公开，请放心添加。</p>
                                </div>
                            </td>
                        </tr>
                    </table>
            </div><!--main-data end-->
            <!-- <div class="btn-box"><input type="submit" value="保存并继续" class="input-submit" /></div> -->
            </form>
        </div><!--fill-data end-->
        <div id="work_panel"  class="fill-data work"  style="display:none">
            <form  id="work_form" method="post" action="fidarks-bl">
            <input type="hidden" value="30" name="stage" />
            <div class="main-data">
            <div class="fill-data-arrow"></div>
                    <table>
                        <tr id="work_cur_tr">
                            <td class="td-name"><em>*</em>当前所在地 : </td>
                            <td>
                            <select class="select cur-plc select-gray" id="currentprovince" name="currentprovince" style="width:80px;" >
                                <option value="">省份</option>

                            </select>
                            <select class="select select-gray" id="currentprovince_city">
                                <option value="">城市</option>
                            </select>
                            <span style="display:none;" id="currentprovince_mes" class="form-error tips"><span>请填写所在地</span></span>
                            <%--
                            <select class="select" id="currentprovince_city_state" style="display:none">
                                <option value="">选择地区</option>
                            </select>
                            --%>
                            <input type="hidden" id="currentprovince_city_code" name="currentcitycode" />
                            <input type="hidden" id="currentprovince_city_name" name="currentcityname" />
                            </td>
                        </tr>
	                        
                        <%-- By bo.hu@opi-corp.com 2010年05月12日 星期三 18时22分23秒 白领隐藏家乡
                        <tr id="work_home_tr">
                            <td class="td-name"><em>*</em>家乡 : </td>
                            <td>
                            <select class="select cur-home select-gray" name="homeprovince" id="homeprovince"  style="width:80px;">
                                <option value="">省份</option>

                            </select>
                            <select class="select select-gray" id="homeprovince_city">
                                <option value="">城市</option>
                            </select>
                            <span style="display:none;" id="homeprovince_mes" class="form-error tips"><span>请填写家乡，会找到老乡</span></span>
                            <input type="hidden" name="homecitycode"  id="homeprovince_city_code"/>
                            <input type="hidden" name="homecityname"  id="homeprovince_city_name"/>
                            </td>
                        </tr>
                        --%>
                        <tr id="work_tr">
                            <td class="td-name" ><em>*</em>单位 : </td>
                            <td><input type="text" class="input-text" id="work" name="companyname" /><span style="display:none;" id="work_mes" class="form-error tips"><span>请填写单位名称，会找到同事</span></span></td>
                            
                        </tr>
                        <tr id="swork_schooltype">
                            <td class="td-name"><em>*</em>毕业学校:</td>
                            <td>
                                <label for="swork-univ">
    
                                    <input type="radio" id="swork-univ" name="schooltype"/>大学
                                </label>
                                <label for="swork-high">
                                    <input type="radio" id="swork-high" name="schooltype"/>高中
                                </label>
                                <label for="swork-tech">
    
                                    <input type="radio" id="swork-tech" name="schooltype"/>中专
                                </label>
                                <label for="swork-junior">
    
                                    <input type="radio" id="swork-junior" name="schooltype"/>初中
                                </label>
                                <label for="swork-entary">
                                     <input type="radio" id="swork-entary" name="schooltype"/>小学
                                </label>
                            </td>
                        </tr>
                        <tr id="swork_univ_tr" style="display:none">
                            <td class="td-name"><em>*</em>大学 : </td>
                            <td>
				                <input type="text" class="input-text  s-select s-univ" id="work_univ" name="universityname" />
				                <input type="hidden" id="work_univ_id" value="0" name="universitycode" />
				                <select class="select select-gray" id="work_univ_year" name="universityyear"><option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="univ_mes" class="form-error tips"><span>请填写大学，会找到大学同学</span></span>
                            </td>
                        </tr>
                        <tr id="swork_univ_dept_type_tr" style="display:none">
	                        <td class="td-name">
	                        </td>
	                        <td>
	    
	                            <select id="work_univ_dept" class="select select-gray univ_dept_select" name="universitydepartment">
	                                <option value="">请选择院系</option>
	
	                            </select>
	                            <select class="select select-gray" name="coursetype" id="work_univ_type">
	                                <option value="">请选择类型</option>
	                                <option value="大学生">本科生</option>
	                                <option value="硕士">硕士</option>
	    
	                                <option value="博士">博士</option>
	                            </select>
	
	                        </td>
                        </tr>

                        <tr id="swork_mid_high_tr" style="display:none">
	                        <td class="td-name" valign="top">
	                            <em>*</em>高中 : 
	                        </td>
	                        <td>
	                        <table>
	                            <tbody>
	                                <tr id="mid_high_true_tr">
	                                    <td>
	                                        <input type="text" id="work_mid_high" name="highschoolname" class="input-text s-select s-high"/>
	                                        <input type="hidden" id="work_mid_high_id" name="highschoolcode"/>
	                                        <select id="work_mid_high_year" name="highschoolyear" class="select select-gray"><option value="">入学年份</option>
	                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
	                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
	                                        <%@ include file="inc/years.inc"%>
			                                </select>
					                        <span style="display:none;" id="univ_high_mes" class="form-error tips"><span>请填写高中，会找到高中同学</span></span>
			                            </td>
                                    </tr>
	                            </tbody>
	                        </table>
	                        </td>
                        </tr>       
                        <!-- 
                        <tr>
                            <td>高一 : <input type="text"  id="grade1"  class="input-text digital" name="highschoolclass1"/> 班　高二 : <input type="text"  id="grade2"  class="input-text digital" name="highschoolclass2" /> 班　高三 : <input type="text"  id="grade3"  class="input-text digital" name="highschoolclass3" /> 班</td>
                        </tr>
                         -->
                        
                        <tr id="swork_mid_junior_tr" style="display:none">
	                        <td class="td-name">
	                            <em>*</em>初中 : 
	                        </td>
	                        <td>
                            <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="work_mid_junior"/>
                            <input type="hidden" id="work_mid_junior_id" name="juniorhighschoolcode" value="0"/>
                            <select class="select select-gray" name="juniorhighschoolyear" id="work_mid_junior_year">
                                <option value="">入学年份</option>
                                    <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                    <c:set var="strutsOptionYearComponent_end" value="1900" />
                                    <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="mid_junior_mes" class="form-error tips"><span>请填写初中，会找到初中同学</span></span>
                            </td>
                        </tr>
                        <tr id="swork_mid_tech_tr" style="display:none">
                            <td class="td-name"><em>*</em>中专: </td>
                            <td>
                                <input type="hidden" value="-1" name="schooltype" id="work_mid_sh_type"/>
                                <!-- <input type="hidden" value="10" name="stage" /> -->
                                <input type="text" class="input-text s-select s-tech" name="techschoolname" id="work_mid_tech"/> 
                                <input type="hidden" id="work_mid_tech_id" value="0" name="techschoolcode"/>

                                <select class="select select-gray"  name="techschoolyear"  id="work_mid_tech_year">
                                        <option value="" >入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                <span style="display:none;" id="mid_tech_mes" class="form-error tips"><span>请填写中专技校，会找到中专技校同学</span></span>
                            </td>
                        </tr>
                        
                        <tr id="swork_mid_entary_tr" style="display:none">
                            <td class="td-name"><em>*</em>小学 : </td>
                            <td>
                                <input type="text" class="input-text" id="work_mid_entary" name="elementaryschoolname"/>
                                <input  type="hidden" id="work_mid_entary_id" name="elementaryschoolcode"/>
                                 <select class="select select-gray" id="work_mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                 </select>
                                 <span  style="display:none;" id="work_mid_entary_mes" class="form-error tips"><span>请填写小学，会找到小学同学</span></span>
                            </td>
                        </tr>
                        
                        <tr class="btn-box">
                            <td></td>
                            <td>
                                <input type="submit" value="保存并继续" class="input-submit" />
                                <div class="register-tips">
                                    <div class="arrow"></div>
    
                                    <p>这些信息会帮助你找到认识的人，<br/>绝不会对陌生人公开，请放心添加。</p>
                                </div>
                            </td>
                        </tr>
                    </table>
            </div><!--main-data end-->
            <!-- <div class="btn-box"><input type="submit" value="保存并继续" class="input-submit" /></div> -->
            </form>

        </div><!--fill-data end-->
        <div id="none_panel" class="fill-data other"  style="display:none">
            <form  id="none_form" method="post" action="fidarks-qt" >
            <input type="hidden" value="90" name="stage" />
            <div class="main-data">
            <div class="fill-data-arrow"></div>
                    <table>
                        <tr id="none_cur_tr">
                            <td class="td-name"><em>*</em>当前所在地 : </td>
                            <td>
                            <select class="select cur-plc select-gray" id="none_currentprovince" name="currentprovince"  style="width:80px;">
                                <option value="">省份</option>
                            </select>
                            <select class="select select-gray" id="none_currentprovince_city">
                                <option value="">城市</option>
                            </select>
                            <span style="display:none;" id="none_currentprovince_mes" class="form-error tips"><span>请填写所在地</span></span>
                            <%--<select class="select select-gray" id="currentprovince_city_state" style="display:none">
                                <option value="">选自地区</option>
                            </select>--%>
                            <input type="hidden" name="currentcitycode"  id="none_currentprovince_city_code"/>
                            <input type="hidden" name="currentcityname"  id="none_currentprovince_city_name"/>

                            </td>
                        </tr>
                        <tr id="none_home_tr">
                            <td class="td-name"><em>*</em>家乡 : </td>
                            <td>
                            <select class="select cur-home select-gray" id="none_homeprovince" name="homeprovince"  style="width:80px;">
                                <option value="">省份</option>
                            </select>
                            <select class="select select-gray"  id="none_homeprovince_city">
                                <option value="">城市</option>
                            </select>
                            <span style="display:none;" id="none_homeprovince_mes" class="form-error tips"><span>请填写家乡，会找到老乡</span></span>
                            <input type="hidden" name="homecitycode"  id="none_homeprovince_city_code"/>
                            <input type="hidden" name="homecityname"  id="none_homeprovince_city_name"/>
                            </td>
                        </tr>
                        <tr id="none_schooltype">
                            <td class="td-name"><em>*</em>毕业学校:</td>
                            <td>
                                <label for="none-univ">
    
                                    <input type="radio" id="none-univ" name="schooltype"/>大学
                                </label>
                                <label for="none-high">
                                    <input type="radio" id="none-high" name="schooltype"/>高中
                                </label>
                                <label for="none-tech">
    
                                    <input type="radio" id="none-tech" name="schooltype"/>中专
                                </label>
                                <label for="none-junior">
    
                                    <input type="radio" id="none-junior" name="schooltype"/>初中
                                </label>
                                <label for="none-entary">
                                     <input type="radio" id="none-entary" name="schooltype"/>小学
                                </label>
                            </td>
                        </tr>
                        <tr id="none_univ_tr" style="display:none">
                            <td class="td-name"><em>*</em>大学 : </td>
                            <td>
                                <input type="text" class="input-text  s-select s-univ" id="none_univ" name="universityname" />
                                <input type="hidden" id="none_univ_id" value="0" name="universitycode" />
                                <select class="select select-gray" id="none_univ_year" name="universityyear">
                                         <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="univ_mes" class="form-error tips">
                                     <span>请填写大学，会找到大学同学</span>
                                </span>
                            </td>
                        </tr>
                        <tr id="none_univ_dept_type_tr" style="display:none">
                            <td class="td-name">
                            </td>
                            <td>
        
                                <select id="none_univ_dept" class="select select-gray univ_dept_select" name="universitydepartment">
                                    <option value="">请选择院系</option>
    
                                </select>
                                <select class="select select-gray" name="coursetype" id="none_univ_type">
                                    <option value="">请选择类型</option>
                                    <option value="大学生">本科生</option>
                                    <option value="硕士">硕士</option>
        
                                    <option value="博士">博士</option>
                                </select>
    
                            </td>
                        </tr>

                        <tr id="none_mid_high_tr" style="display:none">
                            <td class="td-name" valign="top">
                                <em>*</em>高中 : 
                            </td>
                            <td>
                            <table>
                                <tbody>
                                    <tr id="none_mid_high_true_tr">
                                        <td>
                                            <input type="text" id="none_mid_high" name="highschoolname" class="input-text s-select s-high"/>
                                            <input type="hidden" id="none_mid_high_id" name="highschoolcode"/>
                                            <select id="none_mid_high_year" name="highschoolyear" class="select select-gray"><option value="">入学年份</option>
                                            <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                            <c:set var="strutsOptionYearComponent_end" value="1900" />
                                            <%@ include file="inc/years.inc"%>
                                            </select>
                                            <span style="display:none;" id="univ_high_mes" class="form-error tips"><span>请填写高中，会找到高中同学</span></span>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                            </td>
                        </tr>       
                        
                        <tr id="none_mid_junior_tr" style="display:none">
                            <td class="td-name">
                                <em>*</em>初中 : 
                            </td>
                            <td>
                            <input type="text" class="input-text s-select s-junior" name="juniorhighschoolname" id="none_mid_junior"/>
                            <input type="hidden" id="none_mid_junior_id" name="juniorhighschoolcode" value="0"/>
                            <select class="select select-gray" name="juniorhighschoolyear" id="none_mid_junior_year">
                                <option value="">入学年份</option>
                                    <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                    <c:set var="strutsOptionYearComponent_end" value="1900" />
                                    <%@ include file="inc/years.inc" %>
                                </select>
                                <span style="display:none;" id="mid_junior_mes" class="form-error tips"><span>请填写初中，会找到初中同学</span></span>
                            </td>
                        </tr>
                        <tr id="none_mid_tech_tr" style="display:none">
                            <td class="td-name"><em>*</em>中专: </td>
                            <td>
                                <input type="hidden" value="-1" name="schooltype" id="none_mid_sh_type"/>
                                <!-- <input type="hidden" value="10" name="stage" /> -->
                                <input type="text" class="input-text s-select s-tech" name="techschoolname" id="none_mid_tech"/> 
                                <input type="hidden" id="none_mid_tech_id" value="0" name="techschoolcode"/>

                                <select class="select select-gray"  name="techschoolyear"  id="none_mid_tech_year">
                                        <option value="" >入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                <span style="display:none;" id="mid_tech_mes" class="form-error tips"><span>请填写中专技校，会找到中专技校同学</span></span>
                            </td>
                        </tr>
                        
                        <tr id="none_mid_entary_tr" style="display:none">
                            <td class="td-name"><em>*</em>小学 : </td>
                            <td>
                                <input type="text" class="input-text" id="none_mid_entary" name="elementaryschoolname"/>
                                <input  type="hidden" id="none_mid_entary_id" name="elementaryschoolcode"/>
                                 <select class="select select-gray" id="none_mid_entary_year" name="elementaryschoolyear">
                                        <option value="">入学年份</option>
                                        <c:set var="strutsOptionYearComponent_begin" value="<%=java.util.Calendar.getInstance().get(java.util.Calendar.YEAR)%>" />
                                        <c:set var="strutsOptionYearComponent_end" value="1900" />
                                        <%@ include file="inc/years.inc" %>
                                 </select>
                                 <span  style="display:none;" id="none_mid_entary_mes" class="form-error tips"><span>请填写小学，会找到小学同学</span></span>
                            </td>
                        </tr>
                        <tr class="btn-box">
	                        <td></td>
	                        <td>
	                            <input type="submit" value="保存并继续" class="input-submit" />
	                            <div class="register-tips">
	                                <div class="arrow"></div>
	
	                                <p>这些信息会帮助你找到认识的人，<br/>绝不会对陌生人公开，请放心添加。</p>
	                            </div>
	                        </td>
                        </tr>
                    </table>
            </div><!--main-data end-->
            <!-- <div class="btn-box"><input type="submit" value="保存并继续" class="input-submit" /></div>-->
            </form>
        </div><!--fill-data end-->
    </div><!--user-data end-->
</div><!--content end-->
<%
boolean is_the_one = false;
try{
	java.util.Map<String,com.xiaonei.reg.register.model.RegExtraMsg> map = com.xiaonei.reg.register.logic.additional.RegExtraMsgLogic.getInstance().loadExtraMsgMap(com.xiaonei.platform.core.opt.base.BaseThreadUtil.currentHost(request));
	if(map.get("reg_10xinsheng_renren") != null){
		is_the_one = true;
	}
	
}
catch(java.lang.Throwable e){
	
}
%>
<script type="text/javascript">
    var is_the_one = <%=is_the_one%>;
</script>
<%@ include file="/inc/footer-guide-flow.inc" %>

<!-- 组件 -->
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.schoolSelectors.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.currentPlace.js?ver=$revxxx$"></script>
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/xn.ui.pageTools.js?ver=$revxxx$"></script>
<!-- 特定于页面的代码 -->
<script type="text/javascript" src="${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.stormAddStage-v1.3.2.js?ver=$revxxx$"></script>
<script type="text/javascript">
        guideStats.pageid = "p11";
        guideStats.send('ga0');
</script>
<script type="text/javascript">
    XN.dom.ready(function(){
//    	guideStats.send('rggp1v4a0')
        XN.page.guide.stormAddStage.init();
    })
<%--尾号0-2预加载guide js--%>
<c:if test="${ visiter.id%10 == 0 || visiter.id%10 == 1 || visiter.id%10 == 2}">
     preLoadNoExec([
        '${applicationScope.urlSmallStatic}/jspro/xn.app.webpager.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.app.feed.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.app.status.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.app.statushow.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.page.home.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/xn.app.ilike.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.temp-v1.9.js?ver=$revxxx$',
        '${applicationScope.urlSmallStatic}/jspro/guide/xn.page.guide.tab-v1.1.js?ver=$revxxx$'
     ]);
</c:if>
</script>
</body>
</html>
