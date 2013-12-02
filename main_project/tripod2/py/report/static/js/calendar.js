/*
--- Javascript Calendar 
---  By 李双宝
---  Email lovemoon@yeah.net
*/

(function(name,root){
	//写样式
	document.write('<style type="text/css">'	
	+'#'+name+'{position:absolute;border:1px solid #7B9EBD;background-color:#F7F9FB;width:148px;height:160px;overflow:hidden;}'
	+'#'+name+' .'+name+'_turn{cursor:pointer;margin:0 1px;}'
	+'#'+name+' table{background-color:#E0E0E0;z-index:10;}'
	+'#'+name+'_panel{position:relative;margin:5px auto;}'
	+'#'+name+' td{background-color:#F7F9FB;text-align:center;font-family:Arial;font-size:9pt;padding:0.1em 0.2em;cursor:default;color:#003DB8;}'
	+'#'+name+'_th td{background-color:#FFFFFF;}'
	+'#'+name+' td.active_day{background-color:#FFF19F;border:1px solid #FF9900;color:#003DB8;}'
	+'#'+name+' td.disabeld_day{background-color:#EEEEEE;color:#CCCCCC;}'
	+'</style>');	
	
	//全局引用 效率
	var wrap=null;
	var cells=null;
	var title=null;
	//取得对象
	var $=function(id){
		return document.getElementById(id);
	};
	//隐藏日历
	var hide=function(e){
		e=e || window.event;
		var obj=e.target || e.srcElement;
		if(wrap && !isChild(obj,wrap) && obj!=show.activeInput)
			wrap.style.display='none';
	};
	var isTabBlur=function(e){
		e=e || window.event;
		if(e.keyCode==9)
		{
			if(wrap && wrap.style.display=="")
				wrap.style.display="none";
		}
	};
	//判断点击事件源是否为日历控件的子元素
	var isChild=function(inObj,outObj){
		while(inObj)
		{
			if(inObj==outObj)
				return true;
			inObj=inObj.parentNode;
		}
		return false;
	};
	//找到当前给定对象的绝对位置 X Y
	var posX=function(obj){
		return obj.offsetParent ? obj.offsetLeft + posX(obj.offsetParent) : obj.offsetLeft;
	};
	var posY=function(obj){
		return obj.offsetParent ? obj.offsetTop + posY(obj.offsetParent) : obj.offsetTop;
	};	
	//初始化日历
	var init=function(){
		root=root ? root : document.body;
		var html=['<table id="'+name+'_panel" cellpadding="0" cellspacing="1"><thead>','<tr><th colspan="7"><table id="'+name+'_th" width="100%" cellpadding="0" cellspacing="0"><tbody><td><img src="static/images/cal_lt_y.gif" alt="&laquo;" class="'+name+'_turn" onclick="'+name+'_turnYear(\'-\')" title="pre year" /><img src="static/images/cal_lt_m.gif" alt="&lsaquo;" class="'+name+'_turn"  onclick="'+name+'_turnMonth(\'-\')" title="pre month" /></td><td id="'+name+'_title"></td><td><img src="static/images/cal_rt_m.gif" alt="&rsaquo;" class="'+name+'_turn" onclick="'+name+'_turnMonth(\'+\')" title="next month" /><img src="static/images/cal_rt_y.gif" alt="&raquo;" class="'+name+'_turn" onclick="'+name+'_turnYear(\'+\')" title="next year" /></td></tbody></table></td></th>','<tr><td>日</td><td>一</td><td>二</td><td>三</td><td>四</td><td>五</td><td>六</td></tr></thead><tbody id="'+name+'_days">'];
		for(var i=1;i<=6;i++)
		{
			html.push('<tr>');
			for(var j=1;j<=7;j++)
			{				
				var num=7*(i-1)+j;
				html.push('<td>&nbsp;</td>');
			}						
			html.push("</tr>");
		}
		html.push('</tbody></table>');
		//hack ie6 select
		if(navigator.userAgent.toLowerCase().indexOf("msie 6")!=-1)
			html.push('<iframe width="100%" height="100%" frameborder="0" scrolling="no" style="position:absolute;top:0;left:0;z-index:1;filter:"alpha(opacity=0)";opacity:0;"></iframe>');
		
		wrap=document.createElement("div");
		wrap.innerHTML=html.join("");
		wrap.id=name;
		root.appendChild(wrap);
		
		//初始化引用变量
		var daysCon=$(name+"_days");
		cells=daysCon.getElementsByTagName("td");
		title=$(name+'_title');
		//注册事件
		if(document.all)
		{
			//判断显隐日历
			document.attachEvent("onclick",hide);
			document.attachEvent("onkeydown",isTabBlur);
			//监听窗口改变事件
			window.attachEvent("onresize",reLocation);	
			//判断选择了哪一天
			daysCon.attachEvent("onclick",chooseDate);
		}
		else
		{
			document.addEventListener('click',hide,false);
			document.addEventListener("keydown",isTabBlur,false);
			window.addEventListener("resize",reLocation,false);		
			daysCon.addEventListener('click',chooseDate,false);
		}
	};
	//重新调整位置
	var reLocation=function(){
		var obj=show.activeInput;
		wrap.style.left=posX(obj)+"px";
		wrap.style.top=posY(obj)+obj.offsetHeight+"px";
	};
	//显示日历
	var show=window[name]=function(e,args){
		e=e || window.event;
		var obj=e.target || e.srcElement;
		if(!wrap)
			init();
		//记录当前激活的input
		obj.args=args ? args : obj.args;
		show.activeInput=obj;
		wrap.style.left=posX(obj)+"px";
		wrap.style.top=posY(obj)+obj.offsetHeight+"px";
		wrap.style.display='';
		
		renderCalendar(obj.value);
	};
	//组织生成日期数据
	var renderCalendar=function(val){
		//确定初始日期
		var time=new Date();
		if(val.constructor == Date)
			time=val;
		else if(/^\d{4}(-\d{1,2}){2}$/.test(val))
		{
			var temp=val.split("-");
			time=new Date(temp[0],temp[1]-1,temp[2]);
		}
		else if(show.activeInput.args && show.activeInput.args.startDate)
		{
			if(/^\d{4}(-\d{1,2}){2}$/.test(show.activeInput.args.startDate))
			{
				var temp=show.activeInput.args.startDate.split("-");
				time=new Date(temp[0],temp[1]-1,temp[2]);				
			}
			else if(show.activeInput.args.startDate.constructor == Date)
				time=show.activeInput.args.startDate;
		}
		//记录当前日期
		renderCalendar.curDate=time;
		//计算出初始月份第一天是周几
		var temp=new Date(time.getFullYear(),time.getMonth());
		temp.setDate(1);
		var weekDayOfFirst=temp.getDay();
		
		//填充年月
		title.innerHTML=time.getFullYear()+"年"+(time.getMonth()+1) +"月";
		//填充日期
		//当月天数
		var maxDay=haveDays(time);
		//清空 填充空白
		for(var i=0;i<42;i++)
		{
			cells[i].innerHTML="&nbsp;";
			cells[i].className="";
		}
		//进行偏移 weekDayOfFirst
		for(var i=weekDayOfFirst,j=1;i<42&&j<=maxDay;i++,j++)
		{
			cells[i].innerHTML=j;
			if(time.getDate()==j)
				cells[i].className="active_day";
			if(!isValid(j))
				cells[i].className="disabeld_day";				
		}
	};
	//计算给定日期的当前月最大天数
	var daysOfMonth=[31,28,31,30,31,30,31,31,30,31,30,31];
	var haveDays=function(time){
		var year=time.getFullYear();
		var month=time.getMonth();
		//闰年判定
		var isLeapYear=false;
		if((year%4==0 && year%100!=0) || year%400==0)
			isLeapYear=true;
		if(isLeapYear && month==1)
			return 29;
		else
			return daysOfMonth[month];
	};
	//翻月
	var turnMonth=window[name+"_turnMonth"]=function(op){
		var time=renderCalendar.curDate;
		time.setMonth(eval(time.getMonth()+op+1));
		renderCalendar(time);
	};
	//翻年
	var turnMonth=window[name+"_turnYear"]=function(op){
		var time=renderCalendar.curDate;
		time.setFullYear(eval(time.getFullYear()+op+1));
		renderCalendar(time);
	};
	//选择日期
	var chooseDate=function(e){
		e=e || window.event;
		var obj=e.target || e.srcElement;
		if(!/^\d{1,2}$/.test(obj.innerHTML))
			return;
		var time=renderCalendar.curDate;
		if(!isValid(parseInt(obj.innerHTML,10)))
			return;
		show.activeInput.value=time.getFullYear()+"-"+fmtNum(time.getMonth()+1)+"-"+fmtNum(obj.innerHTML);
		wrap.style.display="none";
		//是否有回调
		if(show.activeInput.args && show.activeInput.args.callback)
			show.activeInput.args.callback(show.activeInput.value,new Date(time.getFullYear(),time.getMonth(),obj.innerHTML));
	};
	//将数字格式化为2位
	var fmtNum=function(num){
		return parseInt(num,10)>9 ? num : "0"+num;
	};
	//根据限定条件过滤日期
	var isValid=function(day){	
		var args=show.activeInput.args;
		var time=renderCalendar.curDate;
		var chooseDay=new Date(time.getFullYear(),time.getMonth(),day);
		var selectMill=chooseDay.getTime();
		var webFmt=chooseDay.getFullYear()+"-"+fmtNum(chooseDay.getMonth()+1)+"-"+fmtNum(chooseDay.getDate());
		if(!args)
			return true;
		//是否限定最小日期
		if(args.minDate)
		{
			if(args.minDate.constructor==Date)
			{
				var argsMill=new Date(args.minDate.getFullYear(),args.minDate.getMonth(),args.minDate.getDate()).getTime();
				if(argsMill>selectMill)
					return false;
			}
			else if(/^\d{4}(-\d{1,2}){2}$/.test(args.minDate))
			{
				var fields=args.minDate.split("-");
				var argsMill=new Date(fields[0],fields[1]-1,fields[2]).getTime();
				if(argsMill>selectMill)
					return false;				
			}
		}
		//是否限定最大日期
		if(args.maxDate)
		{
			if(args.maxDate.constructor==Date)
			{				
				var argsMill=new Date(args.maxDate.getFullYear(),args.maxDate.getMonth(),args.maxDate.getDate()).getTime();
				if(argsMill<selectMill)
					return false;
			}
			else if(/^\d{4}(-\d{1,2}){2}$/.test(args.maxDate))
			{
				var fields=args.maxDate.split("-");
				var argsMill=new Date(fields[0],fields[1]-1,fields[2]).getTime();
				if(argsMill<selectMill)
					return false;					
			}
		}
		//用户自定义校验
		if(args.isValid && args.isValid.constructor==Function && !args.isValid(webFmt,new Date(selectMill)))
		{
			return false;
		}
		return true;
	};
})("calendar");