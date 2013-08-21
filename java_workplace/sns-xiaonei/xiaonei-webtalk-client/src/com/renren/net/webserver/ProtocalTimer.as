 package com.renren.net.webserver
 {
	import com.renren.module.IncomingItem;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	
	import mx.rpc.events.FaultEvent;
	import mx.rpc.events.ResultEvent;
	import mx.rpc.http.HTTPService;
	import mx.core.FlexGlobals;
 	public class ProtocalTimer extends EventDispatcher
 	{
		//账号问题
		[Event(name="LogoutError", type="com.renren.net.webserver.ProtocalErrorEvent")]
		//服务端错误
		[Event(name="ServerWarn", type="com.renren.net.webserver.ProtocalErrorEvent")]
		//网络错误
		[Event(name="ConnectWarn", type="com.renren.net.webserver.ProtocalErrorEvent")]
		//服务端错误累积到一定的量
		[Event(name="ServerErr", type="com.renren.net.webserver.ProtocalErrorEvent")]
		//网络错误累积到一定的量
		[Event(name="ConnectErr", type="com.renren.net.webserver.ProtocalErrorEvent")]
		
		//查询到了呼入列表
		[Event(name="ListingSucc", type="com.renren.net.webserver.XnVideoConnectManagerEvent")]
		
		//查询到了当前呼出状态
		[Event(name="HoldingWait", type="ItemEvent")]	//依然等待中
		[Event(name="HoldingMiss", type="ItemEvent")]	//已被拒
		[Event(name="HoldingErr", type="ItemEvent")]	//服务器信息异常
		
		//查询到了当前呼入状态
		[Event(name="RingingWait", type="ItemEvent")]	//对方依然等待中
		[Event(name="RingingMiss", type="ItemEvent")]	//对方已然取消
		[Event(name="RingingErr", type="ItemEvent")]	//服务器信息异常
		
		//通话中,刷新服务器成功
		[Event(name="TalkingSucc", type="com.renren.net.webserver.XnVideoConnectManagerEvent")]
		
		private var _listingservice:HTTPService = null;	//来电查询连接器
		private var _holdingservice:HTTPService = null;	//查询呼出状态连接器
		private var _ringingservice:HTTPService = null;	//查询呼入状态连接器
		private var _talkingservice:HTTPService = null;	//通话中继续链接器
		
		//warn数统计
		private var listingwarncount:int=0;
		private var holdingwarncount:int=0;
		private var ringingwarncount:int=0;
		private var talkingwarncount:int=0;
		//warn数达到这个,就出err
		private const LISTINGERR:int=100;
		private const HOLDINGERR:int=100;
		private const RINGINGERR:int=100;
		private const TALKINGERR:int=100;
		
		public function ProtocalTimer(listingingurl:String,holdingurl:String,ringingurl:String,talkingurl:String):void
		{
			_listingservice = new HTTPService();
			_listingservice.url = listingingurl;
			_listingservice.addEventListener("result", ListingResult);
			_listingservice.addEventListener("fault", ListingConnectWarn);
			
			_holdingservice = new HTTPService();
			_holdingservice.url = holdingurl;
			_holdingservice.addEventListener("result", HoldingResult);
			_holdingservice.addEventListener("fault", HoldingConnectWarn);
			
			_ringingservice = new HTTPService();
			_ringingservice.url = ringingurl;
			_ringingservice.addEventListener("result", RingingResult);
			_ringingservice.addEventListener("fault", RingingConnectWarn);
			
			_talkingservice = new HTTPService();
			_talkingservice.url = talkingurl;
			_talkingservice.addEventListener("result", TalkingResult);
			_talkingservice.addEventListener("fault", TalkingConnectWarn);
		}
		//来电查询
		public function listing(t:String):void
		{
			_listingservice.cancel();
			var request:Object = new Object();
			request.t=t;
			_listingservice.method="POST";
			_listingservice.send(request);
		}
		//查询当前呼出状态,结果是:等待中/已被拒/服务器信息错误
		public function holding(t:String,timeout:int):void
		{
			_holdingservice.cancel();
			var request:Object = new Object();
			request.t=t;
			request.timeout=timeout;
			_holdingservice.method="POST";
			_holdingservice.send(request);
		}
		//查询当前呼入状态,结果是:等待中/对方已取消/服务器信息错误
		public function ringing(userid:String):void
		{
			_ringingservice.cancel();
			var request:Object = new Object();
			request.userid=userid;
			_ringingservice.method="POST";
			_ringingservice.send(request);
		}
		//继续当前通话,延长服务器memcache时间
		public function talking(userid:String,iscaller:String,quality:String, talktime:String,timeout:String):void
		{
			_talkingservice.cancel();
			var request:Object = new Object();
			request.userid=userid;
			request.hostiscaller=iscaller;
			request.quality=quality;
			request.talktime=talktime;
			request.timeout=timeout;
			_talkingservice.method="POST";
			_talkingservice.send(request);
		}
		
		private function ListingResult(e:ResultEvent):void
		{
			listingwarncount = 0;
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				try{
					var users:Array = new Array();
					for each( var userXML:Object in e.result.result ){
						var user:IncomingItem = new IncomingItem(
							userXML.userid,
							userXML.username,
							userXML.tinyurl,
							userXML.mainurl,
							//userXML.status,
							userXML.peer,
							int(userXML.rtmfpok),
							int(userXML.rtmpok),
							userXML.rtmfpurl,
							userXML.rtmpurl);
						users.push(user);
						log("[userXML.rtmpok]"+userXML.rtmpok+"\n");
						log("[int(userXML.rtmpok)]"+int(userXML.rtmpok)+"\n");
					}
					var d:ProtocalListEvent = new ProtocalListEvent("ListingSucc",users);
					dispatchEvent(d);
				}catch(err:Error){
					var drr:Event = new ProtocalErrorEvent("ServerWarn", "ListingResult_result_form_warn");
					dispatchEvent(drr);
				}
			}else{
				drr = new ProtocalErrorEvent("LogoutErr", "ListingResult_no_result_err");
				dispatchEvent(drr);
			}
		}
		private function log(msg:String):void{
			FlexGlobals.topLevelApplication.log(msg);
		}
		private function HoldingResult(e:ResultEvent):void
		{
			holdingwarncount = 0;
			var result:Object = e.result as Object;
			var remote:Object;
			if (result.hasOwnProperty("result"))
			{
				if (result.result.hasOwnProperty("holding"))
				{
					switch(result.result.holding)
					{
						case "WAIT":
							var d:Event = new Event("HoldingWait");
							dispatchEvent(d);
							break;
						case "MISS":
							d = new Event("HoldingMiss");
							dispatchEvent(d);
							break;
						case "ERR":
							d = new Event("HoldingErr");
							dispatchEvent(d);
							break;
						default:
							d = new ProtocalErrorEvent("ServerWarn", "HoldingResult_result_form_warn");
							dispatchEvent(d);
					}
				}
				else
				{
					d = new ProtocalErrorEvent("ServerWarn", "HoldingResult_result_form_warn");
					dispatchEvent(d);
				}
			}
			else
			{
				d = new ProtocalErrorEvent("LogoutError", "HoldingResult_no_result_err");
				dispatchEvent(d);
				
			}
		}
		
		private function RingingResult(e:ResultEvent):void
		{
			ringingwarncount = 0;
			var result:Object = e.result as Object;
			var remote:Object;
			if (result.hasOwnProperty("result"))
			{
				if (result.result.hasOwnProperty("ringing"))
				{
					switch(result.result.ringing)
					{
						case "WAIT":
							var d:Event = new Event("RingingWait");
							dispatchEvent(d);
							break;
						case "MISS":
							d = new Event("RingingMiss");
							dispatchEvent(d);
							break;
						case "ERR":
							d = new Event("RingingErr");
							dispatchEvent(d);
							break;
						default:
							d = new ProtocalErrorEvent("ServerWarn", "RingingResult_result_form_warn");
							dispatchEvent(d);
					}
				}
				else
				{
					d = new ProtocalErrorEvent("ServerWarn", "RingingResult_result_form_warn");
					dispatchEvent(d);
				}
			}
			else
			{
				d = new ProtocalErrorEvent("LogoutError", "HoldingResult_no_result_err");
				dispatchEvent(d);
				
			}
		}
		
		private function TalkingResult(e:ResultEvent):void
		{
			talkingwarncount = 0;
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("talking")){
					var d:Event = new Event("TalkingSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerWarn", "TalkingResult_result_form_warn");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "TalkingResult_no_result_err");
				dispatchEvent(d);
			}
		}
		
		private function ListingConnectWarn(e:FaultEvent):void
		{
			if( ++listingwarncount == LISTINGERR ){
				listingwarncount = 0;
				var d:ProtocalErrorEvent = new ProtocalErrorEvent("ConnectErr", "ListingConnectWarn_http_response_err");
				dispatchEvent(d);
			}else{
				d = new ProtocalErrorEvent("ConnectWarn", "ListingConnectWarn_http_response_warn");
				dispatchEvent(d);
			}
		}
		private function HoldingConnectWarn(e:FaultEvent):void
		{
			if( ++holdingwarncount == HOLDINGERR ){
				holdingwarncount = 0;
				var d:ProtocalErrorEvent = new ProtocalErrorEvent("ConnectErr", "HoldingConnectWarn_http_response_err");
				dispatchEvent(d);
			}else{
				d = new ProtocalErrorEvent("ConnectWarn", "HoldingConnectWarn_http_response_warn");
				dispatchEvent(d);
			}
		}
		private function RingingConnectWarn(e:FaultEvent):void
		{
			if( ++ringingwarncount == RINGINGERR ){
				ringingwarncount = 0;
				var d:ProtocalErrorEvent = new ProtocalErrorEvent("ConnectErr", "RingingConnectWarn_http_response_err");
				dispatchEvent(d);
			}else{
				d = new ProtocalErrorEvent("ConnectWarn", "RingingConnectWarn_http_response_warn");
				dispatchEvent(d);
			}
		}
		private function TalkingConnectWarn(e:FaultEvent):void
		{
			if( ++talkingwarncount == TALKINGERR ){
				talkingwarncount = 0;
				var d:ProtocalErrorEvent = new ProtocalErrorEvent("ConnectErr", "TalkingConnectWarn_http_response_err");
				dispatchEvent(d);
			}else{
				d = new ProtocalErrorEvent("ConnectWarn", "TalkingConnectWarn_http_response_warn");
				dispatchEvent(d);
			}
		}
 	}
 }
