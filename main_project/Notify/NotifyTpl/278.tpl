{{#HEAD}}
<div class="requests-header" id="requests_poke_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/hi.gif">招呼<span>{{278_count}}</span></h2>
</div>
{{/HEAD}}
<link type="text/css" href="http://s.xnimg.cn/a11667/csspro/module/poke.css" />
<script src="http://s.xnimg.cn/a2252/jspro/xn.app.poke.js" type="text/javascript"></script>
<script type="text/javascript">
	(function(){
		ReqCenter.regReqType('poke:278', {
			reply: function(info){
				var target = info.params;
				doPoke('', target, '');
			}
		});
	})();
</script>

<ul id="requests_poke_list" class="requests-list">
{{>LI_278}}
</ul>


{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{278_count}}" onclick="ReqCenter.funCall('req_more', 'poke', this)" class="requests-show-more">显示更多招呼</a>
</div>
{{/MORE}}
