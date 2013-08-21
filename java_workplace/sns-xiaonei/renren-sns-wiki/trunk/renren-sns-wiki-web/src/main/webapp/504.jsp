<%@ page contentType="text/html;charset=UTF-8" language="java" session="false" %>
<!DOCTYPE html>
<html>
    <head>
    	<style>
    		.wiki_error{
    			width:978px;
    			height:500px;
    			margin:auto;
    			position:relative;
    			top:-10px;
    			background: white;
				border: 1px solid #D1D1D1;
				border-radius: 0 0 3px 3px;
				box-shadow: 0 1px 0 lightGrey;
				overflow: hidden;
    		}
    		.wiki_error .banner{
    			height: 230px;
				position: relative;
    			background:url('http://a.xnimg.cn/apps/wiki/images/banner.png');
				background-repeat: no-repeat!important;
				background-position-y:-14px;
    		}
    	</style>
		<title>人人网 校内 - 服务器错误，请稍候再试</title>
		<%@ include file="/inc/n/head2.inc"%>
	</head>
    <body class="error">
    	<%@ include file="/inc/n/header2.inc"%>
        <div id="container">
            <div class="wiki_error">
                <div class="banner">
                </div>
                <div style="position:relative;left:200px;">
                	<img src="http://a.xnimg.cn/apps/wiki/images/504.png"/>
                </div>
            </div>
        </div>
        <%@ include file="/inc/n/footer.inc"%>
    </body>
</html>
