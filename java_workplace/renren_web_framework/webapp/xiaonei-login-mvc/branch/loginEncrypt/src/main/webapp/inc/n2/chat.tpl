{{#WEB_PAGER_CONDITION}}
<div id="console_log" style="position:absolute;top:0;left:0;"></div>
<link type="text/css" rel="stylesheet" href="{{url_small_static}}/{{n_core_webpager-std-min_css_version}}/n/core/webpager-std-min.css"/>
<link type="text/css" rel="stylesheet" href="{{url_small_static}}/{{n_core_webpager-ie6-min_css_version}}/n/core/webpager-ie6-min.css"/><style id="CSSID_9f2766a4"></style>
<link type="text/css" rel="stylesheet" href="{{url_small_static}}/{{n_core_webpager-ie7-min_css_version}}/n/core/webpager-ie7-min.css" />
<script>if(window.ActiveXObject&&!window.XMLHttpRequest)XN.disableWebpager=true;</script>

{{#IS_NOT_NODE_USER}}
<script type="text/javascript">XN.disableWebpager=true</script>
{{/IS_NOT_NODE_USER}}

<iframe id="imengine" name="imengine" src="{{url_wpi}}/wtalk/ime.htm?v=4" frameBorder="0" style="position:absolute;left:-1000pt;top:20pt;width:200pt;height:100pt;"></iframe>

<div id="system-notification-box" style="display:none">
  <article id="system-notification" class="system-notification">
    <section>
      <article class="system-notification-item">
        <header>
          <img class="icon" width="16" height="16" src="{{url_static}}/n/res/placeholder/icon.gif" alt="状态" />
          <a class="x-to-hide" href="#"></a>
        </header>
        <section></section>
      </article>
    </section>
  </article>
</div>
<div id="bottombar"></div>
{{/WEB_PAGER_CONDITION}}
