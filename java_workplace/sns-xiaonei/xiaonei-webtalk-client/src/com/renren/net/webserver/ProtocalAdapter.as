 
 package com.renren.net.webserver
 {
	 import com.renren.module.IncomingItem;
	 
	 import flash.events.Event;
	 import flash.events.EventDispatcher;
	 
	 import mx.core.FlexGlobals;
	 import mx.rpc.events.FaultEvent;
	 import mx.rpc.events.ResultEvent;
	 import mx.rpc.http.HTTPService;
	
 	public class ProtocalAdapter extends EventDispatcher
 	{
		[Event(name="LogoutErr", type="com.renren.net.webserver.ProtocalErrorEvent")]
		[Event(name="ServerErr", type="com.renren.net.webserver.ProtocalErrorEvent")]
		[Event(name="ConnectErr", type="com.renren.net.webserver.ProtocalErrorEvent")]
		[Event(name="ServerWarn", type="com.renren.net.webserver.ProtocalErrorEvent")]
		[Event(name="ConnectWarn", type="com.renren.net.webserver.ProtocalErrorEvent")]
		
		[Event(name="InfoSucc", type="com.renren.net.webserver.ProtocalEvent")]

		[Event(name="CallSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="CallFail", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="AcceptSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="AcceptFail", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="CancelSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="RejectSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="BeHangupSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="ReportSucc", type="com.renren.net.webserver.ProtocalEvent")]
		
		[Event(name="LottoSucc", type="com.renren.net.webserver.ProtocalEvent")]		
		[Event(name="FailedSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="BwCheckSucc", type="com.renren.net.webserver.ProtocalEvent")]
		[Event(name="TimeoutSucc", type="com.renren.net.webserver.ProtocalEvent")]
		
		public function info(webServiceUrl:String, userid:String):void
		{
			if (webServiceUrl.length == 0 || userid.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doInfo_url_userid_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", InfoResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.userid=userid;
			httpService.method="POST";
			httpService.send(request);
		}
		/**
		 * 连接失败
		 * 
		 */
		public function failed(webServiceUrl:String, userid:String, isCaller:Boolean, rtmfpurl:String, rtmpurl:String,rtmfpStage:int,rtmpStage:int, incomingitem:IncomingItem,timeout:int,flashVersion:String):void 
							   
		{
			if (webServiceUrl.length == 0 || userid.length == 0 )
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doFailed_url_userid_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", FailedResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.userid=userid;
			request.hostiscaller=isCaller?"true":"false";
			request.rtmfpurl=rtmfpurl;
			request.rtmpurl=rtmpurl;
			
			request.rtmfpStage=rtmfpStage;
			request.rtmpStage=rtmpStage;
			request.flashVersion=flashVersion;
			
			if( isCaller ){
				var tmp1:int = rtmfpStage==2?1:0;
				var tmp2:int = rtmpStage==3?1:0;
				request.talkStage = 8*tmp1+4*tmp2;
			}else{
				tmp1 = rtmfpStage==3?1:0;
				tmp2 = rtmpStage==3?1:0;
				request.talkStage = 8*incomingitem.rtmfpok+4*incomingitem.rtmpok+2*tmp1+1*tmp2;
			}
			
			request.timeout=timeout;
			
			httpService.method="POST";
			httpService.send(request);
		}
		
		public function call(webServiceUrl:String, userid:String, peer:String, c_timeout:int, r_timeout:int, rtmfpStage:int,rtmpStage:int, rtmfpurl:String, rtmpurl:String):void
							
		{
			if (webServiceUrl.length == 0 || userid.length == 0 || peer.length == 0 )
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doCall_url_userid_peer_rtmfpok_rtmpok_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			//			httpService.url = webServiceUrl+"/"+user+"/"+id+"/"+timeout+"/"+rtmfpok+"/"+rtmpok;
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", CallResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.recieverid=userid;
			request.callerpeer=peer;
			request.c_timeout=c_timeout;
			request.r_timeout=r_timeout;
			request.rtmfpok=rtmfpStage==2?"true":"false";
			request.rtmpok=rtmpStage==2?"true":"false";
			request.rtmfpStage = rtmfpStage;
			request.rtmpStage = rtmpStage;
			
			var tmp1:int = rtmfpStage==2?1:0;
			var tmp2:int = rtmpStage==2?1:0;
			request.talkStage = 8*tmp1+4*tmp2;
			
			request.rtmfpurl=rtmfpurl;
			request.rtmpurl=rtmpurl;
			httpService.method="POST";
			httpService.send(request);
		}
		public function accept(webServiceUrl:String, userid:String, peer:String, timeout:int, rtmfpStage:int, rtmpStage:int, incomingRtmfpOK:int, incomingRtmpOK:int, rtmfpurl:String, rtmpurl:String):void
		                        
		{
			if (webServiceUrl.length == 0 || userid.length == 0 || peer.length == 0 )
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doAccept_url_userid_peer_rtmfpok_rtmpok_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", AcceptResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.callerid=userid;
			request.recieverpeer=peer;
			request.timeout=timeout;
			request.rtmfpok=rtmfpStage==5?"true":"false";
			request.rtmpok=rtmpStage==6?"true":"false";
			
			request.rtmfpStage = rtmfpStage;
			request.rtmpStage = rtmpStage;
			request.rtmfpurl=rtmfpurl;
			request.rtmpurl=rtmpurl;
			
			var tmp1:int = rtmfpStage==5?1:0;
			var tmp2:int = rtmpStage==6?1:0;
			request.talkStage = 8*incomingRtmfpOK+4*incomingRtmpOK+2*tmp1+1*tmp2;
			
			httpService.method="POST";
			httpService.send(request);
		}
		public function cancel(webServiceUrl:String, timeout:int):void
		{
			if (webServiceUrl.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doCancel_url_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", CancelResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.timeout=timeout;
			httpService.method="POST";
			httpService.send(request);
		}
		public function timeout(webServiceUrl:String, timeout:int):void
		{
			if (webServiceUrl.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doTimeout_url_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", TimeoutResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.timeout=timeout;
			httpService.method="POST";
			httpService.send(request);
		}
		public function reject(webServiceUrl:String, userid:String, timeout:int):void
		{
			if (webServiceUrl.length == 0 || userid.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doReject_url_userid_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", RejectResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.userid=userid;
			request.timeout=timeout;
			httpService.method="POST";
			httpService.send(request);
		}
		public function behangup(webServiceUrl:String, userid:String, hostiscaller:String, talktime:String):void
		{
			if (webServiceUrl.length == 0 || userid.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doBeHangup_url_userid_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", BeHangupResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.userid=userid;
			request.hostiscaller=hostiscaller;
			request.talktime=talktime;
			httpService.method="POST";
			httpService.send(request);
		}
		public function report(webServiceUrl:String, talktype:String, type:String, detail:String):void
		{
			if (webServiceUrl.length == 0 || talktype.length == 0 || type.length == 0 || detail.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doReport_url_talktype_type_detail_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url=webServiceUrl;
			httpService.addEventListener("result", ReportResult);
			httpService.addEventListener("fault", ConnectWarn);
			
			var request:Object = new Object();
			request.talktype=talktype;
			request.type=type;
			request.detail=detail;
			httpService.method="POST";
			httpService.send(request);
		}
		public function lotto(webServiceUrl:String, userid:String):void
		{
			if (webServiceUrl.length == 0 || userid.length == 0)
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doLotto_url_userid_err");
				dispatchEvent(e);
				return;
			}
			
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", LottoResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.userid=userid;
			httpService.method="POST";
			httpService.send(request);
		}
		
		public function bwcheck(webServiceUrl:String,action:String,S_C_KB:int,C_S_KB:int):void
		{
			
			if (webServiceUrl.length == 0 )
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doBwCheck_url_err");
				dispatchEvent(e);
				return;
			}
			log("[protocal_bwcheck]_"+action+" "+S_C_KB+" "+C_S_KB+"\n")
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl;
			httpService.addEventListener("result", BwCheckResult);
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			request.action=action;
			request.rtmpS_C_Kbytes=S_C_KB;
			request.rtmpC_S_Kbytes=C_S_KB;
			httpService.method="POST";
			httpService.send(request);
		}
		
		public function debug(webServiceUrl:String,test:String):void
		{
			
			if (webServiceUrl.length == 0 )
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doDebug_url_err");
				dispatchEvent(e);
				return;
			}
			var httpService:HTTPService = new HTTPService();
			httpService.url = webServiceUrl+"/"+test;
			httpService.addEventListener("fault", ConnectErr);
			
			var request:Object = new Object();
			httpService.method="GET";
			httpService.send(request);
		}
		
		public function recordCurrPeerUserIDs(webServiceUrl:String, userID:String,describe:String,isAdd:Boolean):void
		{
			if (webServiceUrl.length == 0 || describe.length == 0 || userID.length == 0 )
			{
				var e:Event = new ProtocalErrorEvent("ServerErr", "doRecordIDS_url_peerID_userID_err");
				dispatchEvent(e);
				return;
			}
			log("[recordCurrPeerUserIDs]userID_"+userID+"\n");
			log("[recordCurrPeerUserIDs]describe_"+describe+"\n");
			
			var httpService:HTTPService = new HTTPService();
			httpService.url=webServiceUrl;
			
			var request:Object = new Object();
			request.userID=userID; //服务端 map 的key
			request.describe=describe;
			request.isAdd=isAdd;
			
			httpService.method="POST";
			httpService.send(request);
		}
		
		private function log(msg:String):void{
			FlexGlobals.topLevelApplication.log(msg);
		}
		private function ConnectErr(e:FaultEvent):void
		{
			log("[ConnectErr_e.statusCode]"+e.statusCode+"\n");
			log("[ConnectErr_e.fault.content]"+e.fault.content+"\n");
			log("[ConnectErr_e.fault.faultDetail]"+e.fault.faultDetail+"\n");
			var d:ProtocalErrorEvent = new ProtocalErrorEvent("ConnectErr", "ConnectErr_http_response_err");
			dispatchEvent(d);
		}
		private function ConnectWarn(e:FaultEvent):void
		{
			log("[ConnectWarn_e.fault.faultDetail]"+e.fault.faultDetail+"\n");
			var d:ProtocalErrorEvent = new ProtocalErrorEvent("ConnectWarn", "ConnectWarn_http_response_warn");
			dispatchEvent(d);
		}
		private function InfoResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("info")){
					var d:ProtocalInfoEvent = new ProtocalInfoEvent("InfoSucc",
						result.result.info.userid,
						result.result.info.username,
						result.result.info.tinyurl,
						result.result.info.mainurl,
						result.result.info.status,
						result.result.info.permission
					);
					dispatchEvent(d);
				}else{
					var err:ProtocalErrorEvent = new ProtocalErrorEvent("ServerErr", "InfoResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				err = new ProtocalErrorEvent("LogoutErr", "InfoResult_no_result_err");
				dispatchEvent(d);
				
			}
		}
		/**
		 * 接受结果后处理
		 * 
		 */
		private function FailedResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("failed")){
					var d:Event = new Event("FailedSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerErr", "failedResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "failedResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function CallResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("call")){
					if (result.result.call == true){
						var d:Event = new Event("CallSucc");
						dispatchEvent(d);
					}else{
						d = new Event("CallFail");
						dispatchEvent(d);
					}
				}else{
					d = new ProtocalErrorEvent("ServerErr", "CallResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "CallResult_no_result_err");
				dispatchEvent(d);
				
			}
		}
		private function AcceptResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("accept")){
					if (result.result.accept == true){
						var d:Event = new Event("AcceptSucc");
						dispatchEvent(d);
					}else{
						d = new Event("AcceptFail");
						dispatchEvent(d);
					}
				}else{
					d = new ProtocalErrorEvent("ServerErr", "AcceptResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "AcceptResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function CancelResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("cancel")){
					var d:Event = new Event("CancelSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerErr", "CancelResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "CancelResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function TimeoutResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("timeout")){
					var d:Event = new Event("TimeoutSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerErr", "TimeoutResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "TimeoutResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function RejectResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("reject")){
					var d:Event = new Event("RejectSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerErr", "RejectResul_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "RejectResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function BeHangupResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("behangup")){
					var d:Event = new Event("BeHangupSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerErr", "BeHangupResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "BeHangupResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function ReportResult(e:ResultEvent):void
		{
			var d:Event = new Event("ReportSucc");
			dispatchEvent(d);
		}
		private function BwCheckResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("bwCheck")){
					var d:Event = new Event("BwCheckSucc");
					dispatchEvent(d);
				}else{
					d = new ProtocalErrorEvent("ServerErr", "bwCheckResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				d = new ProtocalErrorEvent("LogoutErr", "bwCheckResult_no_result_err");
				dispatchEvent(d);
			}
		}
		private function LottoResult(e:ResultEvent):void
		{
			var result:Object = e.result as Object;
			if (result.hasOwnProperty("result")){
				if (result.result.hasOwnProperty("info")){
					var d:ProtocalInfoEvent = new ProtocalInfoEvent("LottoSucc",
						result.result.info.userid,
						result.result.info.username,
						result.result.info.tinyurl,
						result.result.info.mainurl,
						result.result.info.status,
						result.result.info.permission
					);
					dispatchEvent(d);
				}else{
					var err:ProtocalErrorEvent = new ProtocalErrorEvent("ServerErr", "LottoResult_result_form_err");
					dispatchEvent(d);
				}
			}else{
				err = new ProtocalErrorEvent("LogoutErr", "LottoResult_no_result_err");
				dispatchEvent(d);
				
			}
		}
		
 	}
	
 }
