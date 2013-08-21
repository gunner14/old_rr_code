<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<%@ include file="/inc/head-guide-flow.inc" %>
	
    <link href="${applicationScope.urlSmallStatic}/csspro/apps/home-all.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/csspro/apps/guide-new-game1.1.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/csspro/module/news-feeds.css?ver=$revxxx$" rel="stylesheet" type="text/css" media="all" />
    <link href="${applicationScope.urlSmallStatic}/css/dialogpro.css?ver=$revxxx$" rel="stylesheet" type="text/css" />
		
	<title>${domainIdentified["siteName_fillinfo"]} - 与明星们成为好友吧</title>
	
    <style>
        .found-fri-list ul li {
            width: 75px;
        } .ad-bar .block {
            display: none
        } .notice-holder {
            border-right: 2px solid #E1E1E1;
            border-bottom: 2px solid #E1E1E1
        }
        
    </style>

</head>
 <body id="dashboardPage" class="home" g="999_999">
        <!--menu-bar begin-->
		<%@ include file="/inc/header-guide-flow.inc" %>
		<!--menu-bar end--><!--header-not-login.inc end-->
        <div id="opi" class="page-wrapper clearfix">

            <div class="full-page-holder">
                <div class="full-page clearfix">
                	<form action="/addpages?ref=lp_page_finish" method="post">
                		
					<div class="add-stars">
						<h3>看看明星在干什么？先加他们为好友！</h3>
						<div class="stars">
							<div class="stars-head">
								<span class="operation"><input type="checkbox" id="chooseAll" checked="true" /><label for="chooseAll">选择全部</label></span>
								<span class="choosed-num" id="choosed-num">已选择6个</span>

							</div>
							<div class="stars-list">
							
								<% int i = 0; %>
								<c:forEach var="page" items="${pagelist}">
								<% i++; %>
									<div class="stars-item <%=i%2==1?"":"even" %>">
									<span class="operation"><input type="checkbox" id="${page.id}" value="${page.id}" name="pages" checked="true"/><label for="${page.id}">加为好友</label></span>
									<div class="stars-info">
										<span class="avatar" title="先加TA为好友吧，就可以看TA的主页了！">
											<img src="${page.tinyUrl}" alt="${page.name}" />
										</span>

										<div class="stars-detail">
											<p class="name">
												<strong title="先加TA为好友吧，就可以看TA的主页了！">${page.name}</strong>
												<span>${page.fansCount}个好友</span>
											</p>
											<p class="state" title="先加TA为好友吧，就可以看TA的主页了！">
												${page.description}
											</p>

										</div>
									</div>
								</div>
								</c:forEach>
								
							</div>
						</div>
						<div class="sbm">
							<input type="submit" class="input-submit" id="input-submit" value="选择完毕" />
							<!-- <a href="/guide.do?ref=lp_page_skip">跳过这步</a> -->

						</div>
					</div><!--add-stars end-->
                	</form>
                </div>
            </div>
            <div class="blank-bar"></div>
        </div>
        </div>
    </div>
	
    <%@ include file="/inc/footer-guide-flow.inc" %>
    <script type="text/javascript">
    	(function(){
			
			var items = XN.dom.getElementsByClassName('stars-item');
			var checkedNum = items.length;
			
			var toggledChoOne = function(itemEl,tag) { <%--//切换单个是否选中--%>
				var checkEl = itemEl.getElementsByTagName('input')[0];
				checkEl.checked = tag;
			};
			
			var changeChoNum = function( num ){<%--//变换选择的数量--%>
				checkedNum += num;
				$('choosed-num').innerHTML = $('choosed-num').innerHTML.replace(/\d{1,}/,checkedNum);
			};
			
			var choAll = function(){<%--//全选 --%>
				for(var i = 0,il = items.length; i < il ;i ++ ){
					toggledChoOne( items[i], true);
				}
				checkedNum = items.length;
				changeChoNum(0);
			};
			var disChoAll = function(){<%--//取消全选 --%>
				for(var i = 0,il = items.length; i < il ;i ++ ){
					toggledChoOne( items[i], false);
				}	
				checkedNum = 0;
				changeChoNum(0);
			};
			
<%--
//预备为ajax调用			
//			var sendChos = function(){
//				var arr = [], checkEl ,r;
//				for (var i = 0, il = items.length; i < il; i++) {
//					checkEl = itemEl.getElementsByTagName('input')[0];
//					checkEl.checked == true ? arr.push( checkEl.value ) : void(0);
//				}
//				
//				r = arr.join(',');
//				
//				if(r){
//					var xhr = new XN.net.xmlhttp({
//						url:'addpages?pages=123,124,234',
//						method:'GET',
//						data:r,
//						onSuccess:function(r){}
//					});
//				}else{
//					void(0)
//				}
//			}
--%>
			XN.event.addEvent(document, 'click', function(e){
				var e = e || window.event;
				
				var el = XN.event.element(e);
				
				<%--//单个select--%>
				if(el.tagName.toLowerCase() === 'input' && /\d{1,}/.test( el.id )){
					
					if(el.checked == true){
						changeChoNum(1);
					}
					else{
						changeChoNum(-1);
					}
				}
				<%--//选择全部--%>
				if(el.id === 'chooseAll'){
					el.checked == true ? choAll() :  disChoAll();
				}
				
				<%--//统计--%>
				if(el.tagName.toLowerCase() === 'input' && el.type == "checkbox"){
					
					try{
						new XN.net.xmlhttp({
							url:'http://guide.renren.com/xtrs?ref=lp_page_cc'
						})
					}catch(e){}
				}
<%--
				//发送选择的数据
				if(el.tagName.toLowerCase() === 'input' && el.id === 'input-submit'){
					sendChos();
				}
--%>
			});
			
			<%--//更改默认选中的数字--%>
			changeChoNum(0);
		})()
    </script>
	</body>
</html>