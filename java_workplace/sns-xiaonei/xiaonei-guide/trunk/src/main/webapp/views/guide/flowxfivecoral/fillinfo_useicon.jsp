<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <%@ include file="/inc/head-guide-flow.inc" %>
    <link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.1.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <link href="${applicationScope.urlSmallStatic}/csspro/module/school-selector.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all">
    <title>${domainIdentified["siteName_fillinfo"]} - 寻找你认识的人</title>
</head>
<body id="home" class="guide">
<!--menu-bar begin-->
<%@ include file="/inc/header-guide-flow.inc" %>
<!--menu-bar end--><!--header-information-guide.inc end-->
<div id="content">
    <div class="user-data">
        <div class="title titleimg">

            <h1>你好<span>
            <%
                            String name = (String)request.getAttribute("user_name");
                            if(name !=null){
                                out.println(name);
                            }
                            else{
                                out.println("我的朋友");
                            }
            %>
            </span>欢迎来到人人网</h1>
            <p>通过你的真实信息，我们能为你找到同学、同事和家人。</p>
            <img src="${applicationScope.urlSmallStatic}/imgpro/guide/new_title.png"/>
        </div><!--title end-->
        <div class="identity-data">
            <ul class="clearfix" id="choose-identity">
                <li class="me-work">
                    <label for="me-work"><span><input type="radio" name="identity"  id="me-work"/>我工作了</span></label>
                </li>
                <li class="me-university">
                    <label for="me-university"><span><input name="identity"  type="radio" id="me-university"/>我在上大学</span></label>

                </li>   
                <li class="me-secondary">
                    <input name="identity"  type="radio" id="me-secondary"/><label><span id="me-secondary-text">&nbsp;我在上中学</span></label>

                    <div id="choose-box" class="choose-box" style="display:none">
                        <input id="show-junior" type="button" value="初中" class="input-submit" /><br/>
                        <input id="show-high" type="button" value="高中" class="input-submit" /><br/>
                        <input id="show-tech" type="button" value="中专技校" class="input-submit" />
                    </div>
                </li>
                <li class="me-other">
                    <label for="me-other"><span><input type="radio" name="identity"  id="me-other"/>其他身份</span></label>
                </li>
            </ul>

        </div><!--identity-data end-->
        <form action="ficorals" method="post" id="chooseStage">
            <input type="hidden" value="" name="stage" id="stage"/>
            <div class="btn-box" id="btn-box" style="display:none">
                <input type="submit" value="确定" class="input-submit" id="submit-btn" />
            </div>
        </form>
    </div><!--user-data end-->
</div><!--content end-->
<%@ include file="/inc/footer-guide-flow.inc" %>
<%-- 特定于页面的代码 --%>
<script type="text/javascript">
XN.namespace('page.guide.stormAddStage');

XN.page.guide.stormAddStage.hide  = function( ids ){
	if(!ids) return;
	if( isArray(ids) ){
		XN.array.each(ids, function(i, v){
			$(v).hide();			
		});
	}else{
		$(ids).hide();
	}
}

XN.page.guide.stormAddStage.show = function( ids ){
	if(!ids) return;
	if( isArray(ids) ){
		XN.array.each(ids, function(i, v){
			$(v).show();
		});
	}else{
		$(ids).show();
	}
}

XN.page.guide.stormAddStage.stageMap = {
	'junior' : [11,'初中生'],
	'high'	 : [12,'高中生'],
	'tech'	 : [13,'中专生'],
	'univ' 	 : [20,'大学生'],
	'work' 	 : [30,'已工作'],
	'other'  : [90,'其它']
}

XN.page.guide.stormAddStage.setStage = function( stage ){
	var preFixstr = '确认目前的身份是 : ';
	
	XN.log( this.stageMap[stage] );
	$('stage').value  = this.stageMap[stage][0];
<%--//	$('tips').innerHTML = preFixstr + this.stageMap[stage][1];
//	$('tips').show();	--%>
	
	if(10< this.stageMap[stage][0] && this.stageMap[stage][0] < 20){
		$('me-secondary-text').innerHTML = '我在上' + this.stageMap[stage][1].substring(0,2);
	}	

    $('btn-box').show();
}

XN.page.guide.stormAddStage.init = function(){
	var This = this;
	var identities = $('choose-identity').getElementsByTagName('li');
	for(var i = 0,il = identities.length; i < il; i+=1 ){
		(function( index ){
			var el = identities[index];
			var ckRadio = $( el.className );
			
			XN.event.addEvent(el, 'click', function(e){
				if( index !==2 ) ckRadio.checked = true;
				forbidOther(el);
				
				if(/me-secondary/.test(el.className)){
					//This.setStage('other');
				}

				if(/me-university/.test(el.className)){
					This.setStage('univ');
				}
				
				if(/me-work/.test(el.className)){
					This.setStage('work');
				}
				
				if(/me-other/.test(el.className)){
					This.setStage('other');
				}
				
			});
			
			if( i === 2 ){
				el.onmouseover =  function(){
					$('choose-box').show();
				};
				el.onmouseout = function(){
					$('choose-box').hide();
				};
			}
		})(i)
	}
	
	var forbidOther = function(el){
		for (var i = 0, il = identities.length; i < il; i += 1) {
				if( el !== identities[i] ){
					XN.element.addClass(identities[i], 'layer-gray');
				}else{
					XN.element.delClass(identities[i], 'layer-gray');
				}
		}
	};
	
	
	
	XN.event.addEvent($('show-junior'), 'click', function(e){
		XN.event.stop(e);
		$('choose-box').hide();
		forbidOther($('choose-identity').getElementsByTagName('li')[2]);
		
		$('me-secondary').checked = true;
		This.setStage('junior');
	});
	
	XN.event.addEvent($('show-high'), 'click', function(e){
		XN.event.stop(e);
		$('choose-box').hide();
		forbidOther($('choose-identity').getElementsByTagName('li')[2]);
		
		$('me-secondary').checked = true;
		This.setStage('high');
	});
	
	XN.event.addEvent($('show-tech'), 'click', function(e){
		XN.event.stop(e);
		$('choose-box').hide();
		forbidOther($('choose-identity').getElementsByTagName('li')[2]);
		
		$('me-secondary').checked = true;
		This.setStage('tech');
	});
	
	
	
	XN.event.addEvent(document, 'keydown', function(e){
		if(e.keyCode == 13){
			This.formChecker();
		}
	});
}

<%--//guide 统计--%>
var guideStats = {
    send : function( str ){
        var img = new Image();           
        img.src = 'http://guide.' + XN.env.domain + '/xtrs?nur=' + str + '&t=' + Math.random();
    }    
}

XN.dom.ready(function(){
    XN.page.guide.stormAddStage.init();
        
    <%--//到达--%>
    guideStats.send('rggp1v2a0');

    <%--//确定按钮统计--%>
     XN.event.addEvent( $('submit-btn'), 'click', function(){
          guideStats.send('rggp1v2a1')
    });
});
</script>
</body>
</html>
