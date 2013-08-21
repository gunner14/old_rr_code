<script type="text/javascript">
/*
  <%--
  公共使用的统计方法
  使用方法: 
    在任何统计代码开始前 引入这个inc文件  (只引入inc文件不初始化 不会发送任何统计)
    初始化,initGuideTrace( ); 传入当前页面的page_id.  (初始化过程默认发送ga0,到达信号)
    之后页面上的js就可以调用sendGuideTrace(a, f); 发送统计了
  EXP@jicheng.li: 用Image()对象发统计,页面跳转时不会丢失连接(ajax请求在页面跳转时会被取消)
  --%>
*/
sendGuideTrace = XN.func.empty;  //如果没有初始化 就是空函数 不作任何统计
function initGuideTrace(page, silence){
	if(sendGuideTrace != XN.func.empty) return;  //只能被初始化一次

	var p = page;
	sendGuideTrace = function( a , f , b){
		var src = 'http://guide.renren.com/axat?p='+ p +'&a='+ a +'&f='+ (f || '');
		XN.net.sendStats(src);
		//var stat = new Image();  //老代码,测试通过后可以删除了.
		//stat.src = 'http://guide.renren.com/axat?p='+ p +'&a='+ a +'&f='+ (f || '') +'&t='+ Math.random();
		try{  //兼容老的统计
			b();  //回调函数
		}catch(e){XN.log(e)}
	};
	var arrived = silence || false;  //如果想要定制ga0的其他参数,传入true 然后手动发ga0
	if(!arrived){
		sendGuideTrace('ga0');  //初始化的时候默认发送“到达”统计
	}
}
if(typeof page_id != 'undefined'){  //兼容老的统计页面
	initGuideTrace(page_id);
}
</script>