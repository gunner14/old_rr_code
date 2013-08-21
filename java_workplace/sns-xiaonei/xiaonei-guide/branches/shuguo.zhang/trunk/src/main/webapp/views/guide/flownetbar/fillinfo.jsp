<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	<link href="http://s.xnimg.cn/csspro/apps/reg-guide.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
	<!-- view -->
	<title>${domainIdentified["siteName_fillinfo"]} - 寻找感兴趣的人</title>
</head>

<body class="cyberbar">

<script type="text/javascript">
	new Ajax.Request('/autr.do', {
	    method: 'post',
	    parameters: 'i=20301001',
	    onSuccess: function(transport){
	    }
	});
</script>

<div id="container">
	<%@ include file="/views/guide/inc/menu_withif.inc" %>
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
							

						<form id="netBarForm" action="/finbs?ref=nb_arrival" method="post">
							<div class="personal-info">
								<h4 style="font-size:14px;">5秒<span style="font-weight:normal">填写资料。找老乡、找同城好友！</span></h4>

								<p id="err_panel" class="show-error" style="display:none">
									你还没有选择你现在在哪
								</p>
								<p id="" class="show-error" style="display:none">
									你还没有选择你的家乡
								</p>
								<p id="" class="show-error" style="display:none">
									你还没有设置那些人可以浏览你的页面
								</p>
								<div class="pinfo-form">

										<p>
											<label>你现在在哪 : </label>
											<select id="work_cur" name="fi_currentprovince" class="select-short cur-plc">
												<%@ include file="/views/guide/inc/provinces.inc"%>
											</select>
                                            <input id="work_cur_city_name" type="hidden" name="fi_currentcityname" />
											<input id="work_cur_city_code" type="hidden" name="fi_currentcitycode" />
											<select id="work_cur_city" name="" class="select-short" style="display:none">
												<option value="-1" selected="selected">地区</option>
											</select>
										</p>
										
										<p>
											<label>你的家乡是 : </label>

											<select id="myhome" name="fi_homeprovince" class="select-short cur-home">
												<%@ include file="/views/guide/inc/provinces.inc"%>
											</select>
                                            <input id="myhome_city_name" type="hidden" name="fi_homecityname" />
											<input id="myhome_city_code" type="hidden" name="fi_homecitycode" />
											<select id="myhome_city" name="" class="select-short" style="display:none">
												<option value="-1" selected="selected">地区</option>
											</select>
										</p>
										
										<!-- 
										<p>
											<label>谁可以浏览我 : </label>
											<select id="whocan" name="fi_whocanseeme" class="select-long">

												<option value="-1" selected="selected">设置谁可以看到你的页面</option>
												<option value="1">只有我自己可见</option>
												<option value="2">只有我的好友可见</option>
												<option value="3">好友及同城、同学校、同公司的人可见</option>
												<option value="4">所有人可见</option>
											</select>

										</p>
										 -->
										
										<p class="form-action">
											<input id="goon" type="button" class="input-submit" value="保存并开始" />
										</p>
								</div>
							</div>
						</form>	
							


										
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
<script type="text/javascript">
XN.namespace( 'page.netBar' );
XN.page.netBar.checkForm = function(){
	var emsg = {
		work_cur : '你还没有选择你现在在哪 ',
		work_cur_city_code : '你还没有选择你现在那个地区',
		myhome : '你还没有选择你的家乡 ',
		myhome_city_code : '你还没有选择你的家乡所在的地区',
		whocan : '请选择浏览权限'
	}

	var fields = [ 'work_cur' , 'work_cur_city_code' , 'myhome' , 'myhome_city_code' ];
	var epts =  XN.ui.pageTools.filter( fields , [ '' , '请填写小学' , '请填写大学' , '请填写高中', '请填写初中', '请填写中专/技校' , 0 , '0' , '-1' , -1 ] , false );
	
	if( epts.length ){
		$( 'err_panel' ).innerHTML = emsg[ epts[ 0 ].id  ];	
		$( 'err_panel' ).show();
		return false;
	}

	new Ajax.Request('/autr.do', {
         method: 'post',
         parameters: 'i=20301002',
         onSuccess: function(transport){
         }
    });	
	
	$( 'err_panel' ).hide();
	
	$( 'netBarForm' ).submit();
	
	return false;
	
}

XN.dom.ready(function(){

	
	XN.event.addEvent( 'goon' , 'click' , function( e ){
		XN.event.stop( e );
		XN.page.netBar.checkForm();
	});
	
	//var error_panel = new Shower( [ 'err_panel' ] );
});


</script>
<!-- 
<script type="text/javascript" src="http://s.xnimg.cn/jspro/guide/xn.page.guide.addStage.js?ver=$revxxx$"></script>

 -->

</body>
</html>