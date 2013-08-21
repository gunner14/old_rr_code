<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://struts.apache.org/tags-html" prefix="html"%>
<%@ taglib uri="http://www.renren.com/tags-core" prefix="xiaonei"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<!-- _msninit_ -->
<head>
<%@ include file="/inc/head-guide-flow.inc" %>
	<c:set var="INC_NO_JEBE" value="true" />
	<style>
            body,h3,p,h4,ul,li{padding:0;margin:0;font-size:12px}
            ul{list-style:none}
            h3{
                font-size:14px;
                font-weight:normal;
            }
            h4{color:#333;}
            .mini-module h4{
                margin:10px 0;
            }
            img{border:none}
            a{text-decoration:none;color:#005EAC;}
            a:hover{text-decoration:underline;}
            .msn-box{
                width:555px;
                border:1px solid #ced7ef;
                border-top:none;
                margin:0 auto;
                color:#666
                }
            .renrenlogo {
                background:url(${applicationScope.urlStatic}/imgpro/logo/logo-for-activity.png) no-repeat #3777BC;
                height:35px;
                text-align:right;
            }
            .guest-name{
                margin-left: 5px;
                line-height: 35px;
                padding-right:10px;
                font-size:14px;
            }
            .guest-name a{color: #fff!important;}
            .main-content{
                padding:20px 10px 60px;
                background:#FFF url(${applicationScope.urlStatic}/imgpro/MSN/msn-bg.png) bottom left no-repeat;
            }
            .login-bar{
                float:left;
                border:1px solid #CEE1EE;
                background:#F3FAFF;
                padding:15px 5px 10px 15px;
                display:block;  
                width:133px;
            }
            p{
                margin:10px 0;
                line-height:20px;
            }
            .bluebtn{
                background:#005EAC;
                border:1px solid;
                border-color:#b8d4e8 #114680 #114680 #b8d4e8;
                color:#FFF;
                font-weight:bold;
                padding:5px 15px;
                *padding:6px 5px 0;
                font-size:14px;
                margin-left:12px;
                cursor:pointer;
            }
            .list-info{
                margin-left:180px;
            }
            .module{
                border-bottom:1px solid #e3eef8;
                margin-bottom:10px;;
            }
            .module img{
                margin-right:5px;
                vertical-align:middle
            }
            .mini-module{
                float:left;
                width:160px
            }
            .friends{margin-right:-20px;}
            .friends li{
                width:50px;
                margin-right:20px;
                white-space:nowrap;
                overflow:hidden;
                text-align:center;
                font-weight:bold;
                float:left;
            }
            .friends a{
                display:block;
                margin-bottom:5px;}
            .words{
                line-height:16px;
                margin:5px 20px;
            }
            .mini-module img{float:left}
            .footbar{
                clear:both;
                border-top:1px solid #CEE1EE;
                text-align:center;
                padding:10px
            }
            .popbar{color:#cee1ee;margin:0 3px;}
            /* clearFix*/
            .clearfix:after{content:".";display:block;height:0;clear:both;visibility:hidden}
            .clearfix{display:inline-block}
            .clearfix{display:block}
        </style>
    <title>${domainIdentified["siteName_fillinfo"]}</title>
</head>

<body id="home" class="guideActivit">
<c:set var="cleanpage" value="ture" />
        <div class="msn-box">
            <div class="renrenlogo">
                <div style="float:left"><a target="_blank" href="/msninit?a=login"><img src="${applicationScope.urlStatic}/imgpro/logo/logo-for-activity.png" /></a></div>
                <span class="guest-name"><a target="_blank" href="/msninit?a=login">${requestScope.activity_msn_tab_hostname}</a></span>
            </div>
            <div class="main-content clearfix">
                <div class="login-bar">
                    <h3>Hi!${requestScope.activity_msn_tab_hostname}：</h3>

                    <p>欢迎加入人人网大家庭！相信你会喜欢这里。</p>
                    <p><input type="button" class="bluebtn" value="立刻登录" onclick="window.open('/msninit?a=login')" /></p>
                </div>
                <div class="list-info">
                    <div class="module">
                        <h4><img src="${applicationScope.urlStatic}/imgpro/icons/friends.gif" />我们为你找到可能认识的人</h4>
                        <p class="words">一起下过乡，一起扛过枪，一起同过窗的他们，都在人人网等你！</p>

                        <ul class="friends clearfix">
                        <script>
                        var hasAddFriendList = ${requestScope.activity_msn_tab_json_list} || {}
                        XN.dom.ready(function(){
                            var list = hasAddFriendList.candidate || [];
                            for(var i=0,len = list.length; i < 5 ; i++){
                                var obj = list[i];
                                $('user-' + (i+1)).innerHTML = '<a target="_blank" href="/msninit?a=login"><img  style="display: inline-block; overflow: hidden; width: 50px; height: 50px;"  src="'+obj.head+'" alt="'+obj.name+'" /></a><a target="_blank" href="/msninit?a=login">'+obj.name+'</a>'
                            }

                        },true)
                        </script>
                            <li id="user-1">
                                <a target="_blank" href="/msninit?a=login"><img style="display: inline-block; overflow: hidden; width: 50px; height: 50px;" src="http://hdn.xnimg.cn/photos/hdn601/20090202/18/15/tiny_CFOI_5440n198107.jpg" alt="表妹" /></a>
                                <a target="_blank" href="/msninit?a=login">表妹</a>
                            </li>
                            <li id="user-2">
                                <a target="_blank" href="/msninit?a=login"><img style="display: inline-block; overflow: hidden; width: 50px; height: 50px;" src="http://hdn.xnimg.cn/photos/hdn321/20100208/1600/tiny_ERSA_52310o019116.jpg" alt="学姐" /></a>
                                <a target="_blank" href="/msninit?a=login">学姐</a>
                            </li>
                            <li id="user-3">
                                <a target="_blank" href="/msninit?a=login"><img style="display: inline-block; overflow: hidden; width: 50px; height: 50px;" src="http://hdn.xnimg.cn/photos/hdn321/20091215/1025/tiny_m5NT_16516g019116.jpg" alt="哥们" /></a>
                                <a target="_blank" href="/msninit?a=login">哥们</a>
                            </li>
                            <li id="user-4">
                                <a target="_blank" href="/msninit?a=login"><img style="display: inline-block; overflow: hidden; width: 50px; height: 50px;" src="http://hdn.xnimg.cn/photos/hdn421/20100302/2310/tiny_tkSg_162390j019116.jpg" alt="老师" /></a>
                                <a target="_blank" href="/msninit?a=login">老师</a>
                            </li>
                            <li id="user-5">
                                <a target="_blank" href="/msninit?a=login"><img style="display: inline-block; overflow: hidden; width: 50px; height: 50px;" src="http://hdn.xnimg.cn/photos/hdn121/20100206/0135/tiny_CdHc_39567f019118.jpg" alt="同事" /></a>
                                <a target="_blank" href="/msninit?a=login">同事</a>
                            </li>
                        </ul>
                    </div>
                    <div class="module clearfix">

                        <div class="mini-module">
                            <h4><img src="${applicationScope.urlStatic}/imgpro/icons/follow.gif" />关注最热门明星</h4>
                            <a target="_blank" href="/msninit?a=login"><img src="${applicationScope.urlStatic}/imgpro/MSN/pagead.png" /></a><p>春哥新专辑、芙蓉拍电影，你更关注哪个？</p>
                            
                        </div>
                        <div class="mini-module">
                            <h4><img src="${applicationScope.urlStatic}/imgpro/icons/follow.gif" />玩最火爆的游戏</h4>
                            <a target="_blank" href="/msninit?a=login"><img src="${applicationScope.urlStatic}/imgpro/MSN/rrfarm.png"  /></a><p>种菜收菜，养鸡养猪！丰收时刻，谁动了我的西红柿？</p>

                        </div>
                    </div>
                </div>
            </div>
            <div class="footbar">
                <a target="_blank" href="http://tou.live.com/zh-cn/default.aspx?HTTP_HOST=tou.live.com&url=/zh-cn">使用条款</a>
                <span class="popbar">|</span>
                <a target="_blank" href="http://advertising.microsoft.com/china/home">广告服务</a>

                <span class="popbar">|</span>
                <a target="_blank" href="http://go.microsoft.com/fwlink/?LinkId=74170">隐私声明</a>
                <span class="popbar">|</span>
                <a target="_blank" href="http://privacy2.msn.com/anti-spam/zh-cn/default.aspx">发垃圾邮件策略</a>
            </div>
        </div>  

</body>
</html>
