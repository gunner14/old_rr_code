</div></div></div></div>
{{#IS_NOT_FROM_IM}}
<div id="footer">
  <div class="copyright">
  {{#NOT_HIDE_FOOTER_LINK}}
     <span class="float-right">
       <a href="{{url_www}}/siteinfo/about">关于</a><span class="pipe">|</span>
       <a href="{{url_dev}}">开放平台</a><span class="pipe">|</span>
       <a href="{{url_domain}}/mobilelink.do?psf=40002">手机人人</a><span class="pipe">|</span>
       <a href="{{url_domain}}">广告</a><span class="pipe">|</span>
       <a href="http://job.renren.com/">招聘</a><span class="pipe">|</span>
       <a href="{{url_support}}/visitor/helpcenter">客服帮助</a><span class="pipe">|</span>
       <a href="{{url_www}}/siteinfo/privacy">隐私</a>
     </span>
  {{/NOT_HIDE_FOOTER_LINK}}
    <span>人人网<span title="revision$revxxx$; {{host_name}}">&copy;</span>2011&nbsp;&nbsp;<a style="color:#808080" href="http://www.miibeian.gov.cn/" target="_blank">京ICP证090254号</a></span>
  </div>
</div> {{! end div footer}}

<div class="hidden-area"></div>
<script src="{{url_small_static}}/utm/urchin.js" type="text/javascript"></script>
<script type="text/javascript">
(function()
{
	var id = XN.Cookie.get("id");
    if( id && id.slice(-1) == "6" )
    {
        _userv = 0;
        urchinTracker();
    }
})();
</script>

<script src="{{url_small_static}}/a12023/jspro/beacon.js" type="text/javascript"></script>
<script>
  COMSCORE.beacon({
    c1:2,
    c2:6934070,
    c3:"",
    c4:"",
    c5:"",
    c6:"",
    c15:""
  });
</script>
<script>
  <img src="http://b.scorecardresearch.com/p?c1=2&c2=6934070&c3=&c4=&c5=&c6=&c15=&cj=1" />
</noscript>
{{#NO_WEB_PAGER}}
  {{>chat.tpl}}
{{/NO_WEB_PAGER}}
{{/IS_NOT_FROM_IM}}

