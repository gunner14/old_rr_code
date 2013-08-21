<script type="text/javascript">

function showcon(me,count)
{ 
	var t = $("m"+me).className;
	for(var i=1;i<=count;i++){
		$("m"+i).className="cube transparency";
		//alert ;
	}
	if(t.trim()== "cube transparency")
	{
		$("m"+me).className="cube transparency exp";
	} 
	else
	{
		$("m"+me).className="cube transparency";
	}
}
		
		function yc(x){       
		
		var obj = document.getElementById(x);
		if(obj.style.display == "none" || obj.style.display == "" ){
		obj.style.display = "block";}
		else{
		obj.style.display = "none";}
		
		}
		function csvChkForm()
	{
		var filename=document.getElementsByTagName('input').csvFile.value;
		if (filename == "")
		{
			alert('请选择一个CSV导入文件');
			return false;
		}
		var fileLength = filename.length;
		if(fileLength>4 && filename.substring(fileLength-4)!='.cvs'){
			alert('请选择CSV文件!');
			return false;
		}
	return true;	
	}
	
	function chkMSNform()
	{
	var opt = $('importer_webmail');
		for(i=0;i<opt.length;i++)
		{
			if( opt.options[i].selected == true)
			{
				var s = opt.options[i];
				if(s.value == "")
				{
				alert('请选择一个邮箱');
				return false;
				}
			}
			return true;
		}
	}
</script> 

<c:if test="${empty action_forward}">
<c:set var="action_forward" value="searchDispatch"/>
</c:if>

<div id="oak" class="white">

				<div id="single-column" class="morepadding bordertop">

<%
com.xiaonei.platform.core.model.User host = (com.xiaonei.platform.core.model.User)request.getAttribute("host");
boolean isUniv = false;
//if(visiter.getStage() == com.xiaonei.platform.core.opt.permission.define.Stage.STAGE_UNIV)
if(true)
  {
	isUniv=true;
%>
					<!--webmail-->
					
						<div id="m2" class="cube transparency exp">
						<form class="form-invite" method="post" id="webmail" action="${applicationScope.urlInvite}/${action_forward}.do" onsubmit="return chkMSNform()">
						<fieldset>
							
							<legend></legend>
                            <h2 class="nomicon"><a name="tag2" onfocus="this.blur();" onclick="showcon(2,3);" href="#tag2">更快捷的方式找到已经注册${domainIdentified["siteName"]}的朋友</a></h2>
							<div class="formcontent"><p>输入邮箱信息，看看哪些邮箱联系人在${domainIdentified["siteName"]}上，支持126、163、Hotmail、Sina和Yahoo等常用邮箱。</p>
							<p>
								<label>你的邮箱帐号:</label>
<input type="text" class="inputtext" name="uAccount"id="emailAccount" value=""/> @ 
							<select id="importer_webmail" name="importer">
								<option value="">请选择邮箱</option>
								<option value="@hotmail.com">hotmail.com</option>
								<option value="@163.com">163.com</option>
								<option value="@126.com">126.com</option>
								<option value="@sina.com">sina.com</option>
								<option value="@yahoo.com.cn">yahoo.com.cn</option>
								<option value="@gmail.com">gmail.com</option>
								<option value="@live.cn">live.cn</option>
								<option value="@live.com">live.com</option>
								<option value="@msn.com">msn.com</option>	
								<option value="@yahoo.cn">yahoo.cn</option>
								<option value="@yahoo.com">yahoo.com</option>
							</select>
							</p>
							<p>
								<label>你的邮箱密码:</label>
								<input type="password" class="inputtext" name="pwd" id="emailpwd"/>
							</p>
							<p>
								<label class="empty">提交:</label>
								<input type="submit" class="inputsubmit searchbtnico" value="搜索你的好友" />
							</p>
							<p style="height:30px; margin-top: 10px;">
								<label class="empty">注意:</label>
								<span class="alttext">我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。</span>
							</p></div>
                            
						</fieldset>
						</form>
						</div>
				<%}%>
				
				
						<div id="m1" class="cube transparency
						<%if(!isUniv){%>
						exp
						<%}%>
						">
						<c:if test="${domainIdentified['sys.domain']=='renren.com'}">
						<form class="form-invite" method="post" action="${applicationScope.urlInvite}/searchDispatch.do">
						</c:if>
						<c:if test="${domainIdentified['sys.domain']=='kaixin.com'}">
						<form class="form-invite" method="post" action="${applicationScope.urlInvite}/ivtkicpesdsp.do">
						</c:if>
						<fieldset >
							
							<legend></legend>
										<c:choose>
											<c:when test="${action_forward=='searchDispatch'}">
												<%if(isUniv){%>
												<h2 class="msnicon"><a name="tag1" onfocus="this.blur();" onclick="showcon(1,3);" href="#tag1">查找你的MSN好友中有谁注册了${domainIdentified["siteName"]}</a></h2>
												<%}else{%>
												<h2 class="nomicon"><a name="tag1" onfocus="this.blur();" onclick="showcon(1,3);" href="#tag1">更快捷的方式找到已经注册${domainIdentified["siteName"]}的朋友</a></h2>
												<%}%>
											</c:when>
											<c:otherwise>
												<h2 class="msnicon"><a name="tag1" onfocus="this.blur();" onclick="showcon(1,3);" href="#tag1">查找你的MSN好友中有谁注册了${domainIdentified["siteName"]}</a></h2>
											</c:otherwise>
										</c:choose>
                            
							<div class="formcontent"><p id="msn_altmsg">输入你的MSN帐号信息，看看你的MSN好友们有没有注册${domainIdentified["siteName"]}。</p>
							<p>
								<label>你的MSN帐号:</label>
								<input type="text" class="inputtext" name="uAccount" id="uAccount" value=""/>
							</p>
							<p>
								<label>你的MSN密码:</label>
								<input type="password" class="inputtext" name="pwd" id="pwd"/>
							</p>
							<p>
								<label class="empty">提交:</label>
								<input type="submit" class="inputsubmit searchbtnico" value="搜索你的好友" />
							</p>
							<p style="height:30px; margin-top: 10px;">
								<label class="empty">注意:</label>
								<span class="alttext">我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。</span>
							</p>
							</div>
             </fieldset>
			 <input type=hidden name="importer" value="@hotmail.com">
			 </form>
						</div>
					
<%
host = (com.xiaonei.platform.core.model.User)request.getAttribute("host");
//if(visiter.getStage() != com.xiaonei.platform.core.opt.permission.define.Stage.STAGE_UNIV)
if(false)
  {
%>
					<!--webmail-->
					
						<div id="m2" class="cube transparency">
						<c:if test="${domainIdentified['sys.domain']=='renren.com'}">
						<form class="form-invite" method="post" id="webmail" action="${applicationScope.urlInvite}/searchDispatch.do" onsubmit="return chkMSNform()">
						</c:if>
						<c:if test="${domainIdentified['sys.domain']=='kaixin.com'}">
						<form class="form-invite" method="post" id="webmail" action="${applicationScope.urlInvite}/ivtkicpesdsp.do" onsubmit="return chkMSNform()">
						</c:if>
						<fieldset>
							
							<legend></legend>
                            <h2 class="emailicon"><a name="tag2" onfocus="this.blur();" onclick="showcon(2,3);" href="#tag2">查找163等邮箱联系人<span class="alttext">（支持163、126、Sina、Yahoo以及Gmail等常用邮箱）</span></a></h2>
							<div class="formcontent"><p>输入邮箱信息，看看哪些邮件联系人在${domainIdentified["siteName"]}上。</p>
							<p>
								<label>你的邮箱帐号:</label>
<input type="text" class="inputtext" name="uAccount"id="emailAccount" value="" tabIndex=98/> @ 
							<select id="importer_webmail" name="importer">
								<option value="">请选择邮箱</option>
								<option value="@hotmail.com">hotmail.com</option>
								<option value="@163.com">163.com</option>
								<option value="@126.com">126.com</option>
								<option value="@sina.com">sina.com</option>
								<option value="@yahoo.com.cn">yahoo.com.cn</option>
								<option value="@gmail.com">gmail.com</option>
								<option value="@live.cn">live.cn</option>
								<option value="@live.com">live.com</option>
								<option value="@msn.com">msn.com</option>	
								<option value="@yahoo.cn">yahoo.cn</option>
								<option value="@yahoo.com">yahoo.com</option>
							</select>
							</p>
							<p>
								<label>你的邮箱密码:</label>
								<input type="password" class="inputtext" name="pwd" id="emailpwd" tabIndex=99/>
							</p>
							<p>
								<label class="empty">提交:</label>
								<input type="submit" class="inputsubmit searchbtnico" value="搜索你的好友" />
							</p>
							<p style="height:30px; margin-top: 10px;">
								<label class="empty">注意:</label>
								<span class="alttext">我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。</span>
							</p></div>
                            
						</fieldset>
						</form>
						</div>
<%}%>
					<!--csvinvite-->
					
					<div id="m3" class="cube transparency">
					<form class="form-invite" method="post" action="${applicationScope.urlInvite}/${action_forward}.do" enctype="multipart/form-data">
					<fieldset>
						<legend></legend>
						<h2 class="csvicon"><a name="tag3" onfocus="this.blur();" onclick="showcon(3,3);" href="#tag3">查找Outlook或Foxmail邮箱的联系人</a></h2>
						<div class="formcontent"><p>如果你使用Outlook等邮件工具收发邮件，可以通过上传通讯录文件（csv文件）导入联系人名单来寻找在${domainIdentified["siteName"]}注册的朋友们。</p>
                        <a href="javascript:yc('explainlist1');">怎样创建.csv文件？</a>
                        <ul id="explainlist1" class="explainlist1">
                        	<li>
                            	<a href="javascript:yc('exp1');">Microsoft Outlook</a>
                                <div id="exp1" class="expbox">
                                	<p>通过Microsoft Outlook创建csv文件:</p> 
                                    <ol>
                                    	<li>打开“Outlook”。</li>
                                        <li>在“文件”菜单中选择“导入和导出”。</li>
                                        <li>选择“导出到文件”，单击下一步。</li>
                                          <li>选择“以逗号为分隔符(Windows)”，单击下一步。</li>
                                           <li>选择“联系人”，单击下一步。</li>
                                            <li>选择一个位置保存文件，单击下一步完成。</li>
                                       </ol>
                                    <p>要上传.csv文件，只需单击“浏览”，选择之前创建的文件即可。</p></div></li>
                            <li>
                            	<a href="javascript:yc('exp2');">Outlook Express</a>
                                <div id="exp2" class="expbox">
                                	<p>通过Outlook Express创建csv文件:</p>
                                    <ol>
                                    	<li>打开“Outlook Express”。</li>
                                        <li>在“文件”菜单中选择“导出”“通讯录”。</li>
                                        <li>选择“文本文件”，单击“导出”。</li>
                                        <li>在弹出框中选择csv文件保存的位置和文件名（例如“通讯录.csv”保存到“桌面”），单击“下一步”。</li>
                                        <li>选择要导出的域，请至少选择“姓名”和“电子邮件地址”，单击“完成”。</li></ol>
                                    <p>要上传.csv文件，只需单击“浏览”，选择之前创建的文件即可。</p></div></li>
                            <li>
                            	<a href="javascript:yc('exp3');">Mozilla Thunderbird</a>
                                <div id="exp3" class="expbox">
                                	<p>通过Mozilla Thunderbird创建csv文件:</p>
                                    <ol>
                                    	<li>打开“Mozilla Thunderbird”。</li>
                                        <li>在“工具”菜单中选择“通讯录”。</li>
                                        <li>“通讯录”窗口打开后，在“工具”菜单中选择“导出”。</li>
                                        <li>在弹出框中选择csv文件保存的位置和文件夹的名字，单击“保存”。</li>
                                        <li>选择要导出的域，请至少选择“姓名”和“电子邮件地址”，单击“完成”。</li></ol>
                                    <p>要上传.csv文件，只需单击“浏览”，选择之前创建的文件即可。</p></div></li>
                            <li>
                            	<a href="javascript:yc('exp4');">Foxmail</a>
                                <div id="exp4" class="expbox">
                                	<p>通过Foxmail创建csv文件:</p>
                                    <ol>
                                    	<li>打开“Foxmail”。</li>
                                        <li>点击“地址簿”。</li>
                                        <li>在“工具”菜单中选择“导出”“csv文件”。</li>
                                        <li>在弹出的“向导”框中选择csv文件保存的位置和文件夹的名字，单击“下一步”。</li>
                                        <li>选择你要输出的字段，请至少选择“姓名”和“电子邮件地址”，单击“完成”。</li></ol>
                                    <p>要上传.csv文件，只需单击“浏览”，选择之前创建的文件即可。</p></div></li>
                                    	</ul>
                                    
                            <p>
								<label>提交CSV文件:</label>
								<input type="file" name="csvFile" />
								</p>
								<p><label class="empty">提交:</label>
								<input type="submit" class="inputsubmit searchbtnico" value="搜索你的好友" onClick="javascript:if(csvChkForm()) {return true;} else {return false;}"/>
							</p>
							<p>
								<label class="empty">注意:</label>
								<span class="alttext">我们不会记录你的帐号密码或者未经允许向你的好友发送邮件。</span></p></div>
					</fieldset>
					</form>
					</div>
					

				</div>
				</div>