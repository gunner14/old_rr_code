package com.renren.module
{
	import com.renren.module.VcEvent;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	import flash.net.NetConnection;
	import mx.core.FlexGlobals;
	import flash.net.Responder;
	import flash.external.ExternalInterface;
	
	public class Vctrl extends EventDispatcher
	{
		
		//vc caller 两个都成功
		[Event(name="vc_c_succ", type="com.renren.module.VcEvent")]
		//vc caller 只有rtmfp成功
		[Event(name="vc_c_rtmfp", type="com.renren.module.VcEvent")]
		//vc caller 只有rtmp成功
		[Event(name="vc_c_rtmp", type="com.renren.module.VcEvent")]
		//vc caller 两个都失败
		[Event(name="vc_c_timeout", type="com.renren.module.VcEvent")]
		
		
		//vs caller rtmp 流接收成功
		[Event(name="vs_c_rtmp", type="com.renren.module.VsEvent")]
		
		//vc receiver rtmfp 成功
		[Event(name="vc_r_rtmfp", type="com.renren.module.VcEvent")]
		//vc receiver rtmfp 失败
		[Event(name="vc_r_rtmfp_timeout", type="com.renren.module.VcEvent")]
		
		//vc receiver rtmp 成功
		[Event(name="vc_r_rtmp", type="com.renren.module.VcEvent")]
		//vc receiver rtmp 失败
		[Event(name="vc_r_rtmp_timeout", type="com.renren.module.VcEvent")]
		
		
		//vc_unkown_fail add by tangxm
		[Event(name="vc_unkown_fail", type="com.renren.module.VcEvent")]
		
		
		
		//vs receiver rtmfp流超时
		[Event(name="vs_r_rtmfp_timeout", type="com.renren.module.VsEvent")]
		//vs receiver rtmp流超时
		[Event(name="vs_r_rtmp_timeout", type="com.renren.module.VsEvent")]
		
		
		//vs receiver rtmp 流接收成功
		[Event(name="vs_r_rtmp", type="com.renren.module.VsEvent")]
		
		//vs rtmfp 被对方关闭了
		[Event(name="vs_rtmfp_close", type="com.renren.module.VsEvent")]
		//vs rtmfp 被关闭了
		[Event(name="vs_rtmp_close", type="com.renren.module.VsEvent")]
		
		///////////////////////////////////////////////////
		
		private var _VCRtmfpOk:Boolean;
		private var _VCRtmpOk:Boolean;
		private const VCStatusTimeout:int = 0;
		private const VCStatusRtmfpSucc:int = 1;
		private const VCStatusRtmfpFail:int = 2;
		private const VCStatusRtmpSucc:int = 3;
		private const VCStatusRtmpFail:int = 4;
		
		private var VCTarget:int;
		private const VCTargetCaller:int = 0;
		private const VCTargetReceiverRtmp:int = 1;
		private const VCTargetReceiverRtmfp:int = 2;
		
		private var VGoingDown:int;
		private const VGoingDownTwo:int = 2;
		private const VGoingDownOne:int = 1;
		private const VGoingDownDone:int = 0;
		private const VGoingDownTimeout:int = -1;
		
		private const vctimercron:int = 5;
		private var vctimer:Timer = null;
		
		///////////////////////////////////////////////////
		
		private const VSStatusTimeout:int = 0;
		private const VSStatusRtmfpSucc:int = 1;
		private const VSStatusRtmfpFail:int = 2;
		private const VSStatusRtmpSucc:int = 3;
		private const VSStatusRtmpFail:int = 4;
		
		private const vstimercron:int = 5;
		private var vsrtmfptimer:Timer = null;
		private var vsrtmptimer:Timer = null;
		
		private var checkrtmptidimer:Timer = null;
		
		private var _VTalkingType:int;
		private const _VTalkingTypeNone:int = 0;
		private const _VTalkingTypeRtmfp:int = 1;
		private const _VTalkingTypeRtmp:int = 2;
		
		private var _rtmfpStage :int = 0;
		private var _rtmpStage :int = 0;
		private var _rtmpS_C_Kbytes :int = 0; //rtmp,server到client的带宽检测,单位 kB/s
		private var _rtmpC_S_Kbytes :int = 0; //rtmp,client到server的带宽检测,单位 kB/s

		public function get rtmpC_S_Kbytes():int
		{
			return _rtmpC_S_Kbytes;
		}
		
		public function set rtmpC_S_Kbytes(value:int):void
		{
			_rtmpC_S_Kbytes = value;
		}
		
		public function get rtmpS_C_Kbytes():int
		{
			return _rtmpS_C_Kbytes;
		}
		
		public function set rtmpS_C_Kbytes(value:int):void
		{
			_rtmpS_C_Kbytes = value;
		}
		public function get rtmpStage():int
		{
			return _rtmpStage;
		}

		public function set rtmpStage(value:int):void
		{
			_rtmpStage = value;
		}

		public function get rtmfpStage():int
		{
			return _rtmfpStage;
		}

		public function set rtmfpStage(value:int):void
		{
			_rtmfpStage = value;
		}

		public function Vctrl(){
			vctimer = new Timer(1000 * vctimercron);
			vctimer.addEventListener(TimerEvent.TIMER,event_vc_timeout);
			
			vsrtmfptimer = new Timer(1000 * vstimercron);
			vsrtmfptimer.addEventListener(TimerEvent.TIMER,event_vs_r_rtmfp_timeout);
			
			vsrtmptimer = new Timer(1000 * vstimercron);
			vsrtmptimer.addEventListener(TimerEvent.TIMER,event_vs_r_rtmp_timeout);
			
			_VTalkingType=VTalkingTypeNone;
			VGoingDown=VGoingDownTwo;
			VCTarget=VCTargetCaller;
			_VCRtmfpOk=false;
			_VCRtmpOk=false;
		}
		
		public function reset():void{
			if(vctimer)vctimer.stop();
			if(vsrtmfptimer)vsrtmfptimer.stop();
			if(vsrtmptimer)vsrtmptimer.stop();
			
			_VTalkingType=VTalkingTypeNone;
			VGoingDown=VGoingDownTwo;
			VCTarget=VCTargetCaller;
			_VCRtmfpOk=false;
			_VCRtmpOk=false;
		}
		
		public function vcgoing(connectStatus:int):void
		{
			switch(connectStatus){
				case VCStatusTimeout:
					log("[vcgoing]VCStatusTimeout_"+VGoingDown+"\n");
					if( VGoingDown>VGoingDownDone ){
						VGoingDown=VGoingDownTimeout;
						break;
					}
					log("[vcgoing]sync_warn\n");
					return;
				case VCStatusRtmfpSucc:
					log("[vcgoing]VCStatusRtmfpSucc_"+VGoingDown+"\n");
					if( VGoingDown>VGoingDownDone ){
						--VGoingDown;
						_VCRtmfpOk=true
						break;
					}
					log("[vcgoing]sync_warn\n");
					return;
				case VCStatusRtmfpFail:
					log("[vcgoing]VCStatusRtmfpFaild_"+VGoingDown+"\n");
					if( VGoingDown>VGoingDownDone ){
						--VGoingDown;
						_VCRtmfpOk=false;
						break;
					}
					log("[vcgoing]sync_warn\n");
					return;
				case VCStatusRtmpSucc:
					log("[vcgoing]VCStatusRtmpSucc_"+VGoingDown+"\n");
					if( VGoingDown>VGoingDownDone ){
						--VGoingDown;
						_VCRtmpOk=true
						break;
					}
					log("[vcgoing]sync_warn\n");
					return;
				case VCStatusRtmpFail:
					log("[vcgoing]VCStatusRtmpFaild_"+VGoingDown+"\n");
					if( VGoingDown>VGoingDownDone ){
						--VGoingDown;
						_VCRtmpOk=false
						break;
					}
					log("[vcgoing]sync_warn\n");
					return;
				default:
					log("[vcgoing]unknown_err\n");
					return;
			}
			if( VGoingDown<=VGoingDownDone ){
				vctimer.stop();
				if( VCTarget==VCTargetCaller ){
					if( _VCRtmfpOk && _VCRtmpOk ){
						var d:VcEvent = new VcEvent("vc_c_succ");
						dispatchEvent(d);
					}else if( _VCRtmfpOk ){
						d = new VcEvent("vc_c_rtmfp");
						dispatchEvent(d);
					}else if( _VCRtmpOk ){
						d = new VcEvent("vc_c_rtmp");
						dispatchEvent(d);
					}else{
						d = new VcEvent("vc_c_timeout");
						dispatchEvent(d);
					}
				}else if( VCTarget==VCTargetReceiverRtmfp ){
					if( _VCRtmfpOk ){
						d = new VcEvent("vc_r_rtmfp");
						dispatchEvent(d);
					}else{
						d = new VcEvent("vc_r_rtmfp_timeout");
						dispatchEvent(d);
					}
				}else if( VCTarget==VCTargetReceiverRtmp ){
					if( _VCRtmpOk ){
						d = new VcEvent("vc_r_rtmp");
						dispatchEvent(d);
					}else{
						d = new VcEvent("vc_r_rtmp_timeout");
						dispatchEvent(d);
					}
				}else{
					d = new VcEvent("vc_unkown_fail");
					dispatchEvent(d);
				}
			}
		}
		
		public function vc_timer_start():void{
			vctimer.start();
		}
		public function vc_timer_stop():void{
			vctimer.stop();
		}
		
		/**
		 * 功能:	vc连接前，先设定本次连接的身份为caller，连接类型为两个都连
		**/
		public function ready_caller():void{
			
			VGoingDown=VGoingDownTwo;
			VCTarget=VCTargetCaller;
		}
		/**
		 * 功能:	vc连接前，先设定本次连接的身份为receiver，连接类型为rtmfp
		 **/
		public function ready_receiver_rtmfp():void{
			VGoingDown=VGoingDownOne;
			VCTarget=VCTargetReceiverRtmfp;
		}
		/**
		 * 功能:	vc连接前，先设定本次连接的身份为receiver，连接类型为rtmp
		 **/		
		public function ready_receiver_rtmp():void{
			VGoingDown=VGoingDownOne;
			VCTarget=VCTargetReceiverRtmp;
		}
		
		
		public function vc_rtmfp_fail():void
		{
			log("[vc_rtmfp_fail]...\n");
			vcgoing(VCStatusRtmfpFail);
		}
		
		public function vc_rtmp_fail():void
		{
			log("[vc_rtmp_fail]...\n");
			vcgoing(VCStatusRtmpFail);
		}
		
		public function vc_rtmfp_succ(peer:String):void
		{
			log("[vc_rtmfp_succ]peer_is_"+peer+"\n");
			vcgoing(VCStatusRtmfpSucc);
		}
		
		public function vc_rtmp_succ():void
		{
			log("[vc_rtmp_succ]...\n");
			vcgoing(VCStatusRtmpSucc);
		}
		public function vc_rtmfp_close():void
		{
			log("[vc_rtmfp_close]...\n");
		}
		
		public function vs_succ_rtmfp(farID:String):void
		{
            log("[vs_succ_rtmfp]farID_"+farID+"\n");
		}
		public function vs_closed_rtmfp():void
		{
			log("[vs_closed_rtmfp]...\n");
			var d:VsEvent = new VsEvent("vs_rtmfp_close");
			dispatchEvent(d);
		}
		
		public function vs_succ_rtmp():void
		{
			log("[vs_succ_rtmp]...\n");
			if( VCTarget==VCTargetCaller ){
				var d:VsEvent = new VsEvent("vs_c_rtmp");
				dispatchEvent(d);
			}else{
				d = new VsEvent("vs_r_rtmp");
				dispatchEvent(d);
			}
		}
		public function vs_closed_rtmp():void
		{
			log("[vs_closed_rtmp]...\n");
			var d:VsEvent = new VsEvent("vs_rtmp_close");
			dispatchEvent(d);
		}
		
		public function vc_closed_rtmfpconn(localstatus:LocalStatus):void
		{
			log("[vc_closed_rtmfpconn]...\n");
			
			if(localstatus.iscaller())
			{
				FlexGlobals.topLevelApplication.movePeerUserIDs();
			}
		}
		
		public function vs_rtmp_timer_start():void{
			vsrtmptimer.start();
		}
		public function vs_rtmp_timer_stop():void{
			vsrtmptimer.stop();
		}
		public function vs_rtmfp_timer_start():void{
			vsrtmfptimer.start();
		}
		public function vs_rtmfp_timer_stop():void{
			vsrtmfptimer.stop();
		}
		/**
		 * vc通道连接时限已到
		 * 功能:	进入vcgoing进行状态变更,抛出相应事件
		 */
		private function event_vc_timeout(e:TimerEvent):void
		{
			log("[event_vc_timeout]"+vctimercron+"_secs\n");
			vcgoing(VCStatusTimeout);
		}
		/**
		 * rtmfp视频流连接时限已到
		 * 功能:	抛出相应事件
		 */
		private function event_vs_r_rtmfp_timeout(e:TimerEvent):void
		{
			log("[event_vs_r_rtmfp_timeout]"+vstimercron+"_secs\n");
			vsrtmfptimer.stop();
			var d:VsEvent = new VsEvent("vs_r_rtmfp_timeout");
			dispatchEvent(d);
		}
		/**
		 * rtmp视频流连接时限已到
		 * 功能:	抛出相应事件
		 */
		private function event_vs_r_rtmp_timeout(e:TimerEvent):void
		{
			log("[event_vs_r_rtmp_timeout]"+vstimercron+"_secs\n");
			vsrtmptimer.stop();
			var d:VsEvent = new VsEvent("vs_r_rtmp_timeout");
			dispatchEvent(d);
		}

		public function get VTalkingType():int
		{
			return _VTalkingType;
		}
		
		public function rtmfp_talking():void{
			_VTalkingType=VTalkingTypeRtmfp;
		}
		
		public function rtmp_talking():void{
			_VTalkingType=VTalkingTypeRtmp;
		}
		
		public function is_rtmfp_talking():Boolean{
			
			return _VTalkingType==VTalkingTypeRtmfp;
		}
		
		public function is_rtmp_talking():Boolean{
			
			return _VTalkingType==VTalkingTypeRtmp;
		}
		
		private function log(msg:String):void{
			FlexGlobals.topLevelApplication.log(msg);
		}

		public function get VTalkingTypeNone():int
		{
			return _VTalkingTypeNone;
		}

		public function get VTalkingTypeRtmfp():int
		{
			return _VTalkingTypeRtmfp;
		}

		public function get VTalkingTypeRtmp():int
		{
			return _VTalkingTypeRtmp;
		}


	}
}