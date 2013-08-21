package com.renren.module
{
	import flash.events.ActivityEvent;
	import flash.events.StatusEvent;
	import flash.events.TimerEvent;
	import flash.media.Camera;
	import flash.media.Microphone;
	import flash.media.SoundCodec;
	import flash.net.NetStreamInfo;
	import flash.net.SharedObject;
	import flash.system.Capabilities;
	import flash.utils.Timer;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	import mx.collections.ArrayCollection;
	import mx.controls.VideoDisplay;
	import mx.core.Application;
	import mx.core.FlexGlobals;
	
	public class LocalEquip {
		
		private const _CamerastatusInit:String=		"";
		private const _CamerastatusNotfound:String=	"没有找到摄像头";
		private const _CamerastatusMuted:String=		"您未授权使用设备";
		private const _CamerastatusUnmuted:String=		"视频采集中...";
		private const _CamerastatusWork:String=		"摄像头正常";
		private const _CamerastatusBusy:String=		"摄像头被占用";
		private var _Camerastatus:String;
		
		private const _MicstatusInit:String=			"";
		private const _MicstatusNotfound:String=		"没有找到音频设备";
		private const _MicstatusMuted:String=			"您未授权使用设备";
		private const _MicstatusWork:String=			"音频设备正常";
		private var _Micstatus:String;
		
		private const _CameractrlNotAv:String=			"无摄像头";
		private const _CameractrlEnable:String=		"已开启摄像头";
		private const _CameractrlDisable:String=		"已暂停摄像头";
		private var _Cameractrl:String;
		
		private const _MicctrlNotAv:String=			"无音频设备";
		private const _MicctrlEnable:String=			"已开启音频设备";
		private const _MicctrlDisable:String=			"已暂停音频设备";
		private var _Micctrl:String;
		
		private var _PlayerSmallIndex:int;						//本地视频区
		private const _PlayerSmallVideoSelected:int = 0;				//摄像头正常时显示
		private const _PlayerSmallNoVideoSelected:int = 1;				//摄像头不正常时显示
		private const _PlayerSmallHidden:int = 2;
		
		private var _PlayerOperationTalkingCameraIndex:int;	//通话时的用户操作区中的Camera
		private const _PlayerOperationTalkingCameraOnSelected:int = 0;	//Camera开
		private const _PlayerOperationTalkingCameraOffSelected:int = 1;//Camera关
		
		private var _PlayerOperationTalkingMicIndex:int;		//通话时用户操作区中的Mic
		private const _PlayerOperationTalkingMicOnSelected:int = 0;		//Mic开
		private const _PlayerOperationTalkingMicOffSelected:int = 1;		//Mic关
		
//		private const defaultMacCamera:String = "USB Video Class Video";
		private var _localSO:SharedObject;
		
		[Bindable] private var micNames:Array;
		private var _micindex:int=0;
		[Bindable] private var cameraNames:Array;
		private var _cameraindex:int=0;
		private var _micvolume:int=50;
		private var _speakervolume:Number=0.8;
		
		private var _micnames:Array;
		private var _cameranames:Array;
		
		private var _avmic:Microphone=null;
		private var _avcamera:Camera=null;
		
		private var audioRate:Array = new Array(30);
		private var audioRateDisplay:ArrayCollection = new ArrayCollection();
		private var videoRate:Array = new Array(30);
		private var videoRateDisplay:ArrayCollection = new ArrayCollection();
		private var srtt:Array = new Array(30);
		private var srttDisplay:ArrayCollection = new ArrayCollection();
		
		private const AvCheckCron:int = 500;
		private const AvCheckCount:int = 10;
		private var avchecktimer:Timer = new Timer(AvCheckCron,AvCheckCount);
		
		private var _callback:Function;
		
		public function LocalEquip(localSOname:String){
			
			_localSO=SharedObject.getLocal(localSOname);
			
			var mics:Array=Microphone.names;
			if (mics){
				micNames=mics;
			}
			
			var cameras:Array=Camera.names;
			if (cameras){
				cameraNames=cameras;
			}
			
			if (_localSO.data.hasOwnProperty("micIndex"))
			{
				_micindex=_localSO.data.micIndex;
			}
			log("[LoaclEquip_Capabilities.os]"+Capabilities.os+"\n");
			// selected camera device
			if (_localSO.data.hasOwnProperty("cameraIndex"))
			{
				_cameraindex=_localSO.data.cameraIndex;
			}
			// mic volume
			if (_localSO.data.hasOwnProperty("micVolume"))
			{
				_micvolume=_localSO.data.micVolume;
			}
			// speaker volume
			if (_localSO.data.hasOwnProperty("speakerVolume"))
			{
				_speakervolume=_localSO.data.speakerVolume;
			}
			
			avchecktimer.addEventListener(TimerEvent.TIMER,avcheck);
			this.Micstatus=_MicstatusInit;//""
			this.Micctrl=_MicctrlNotAv;
			this.Camerastatus=_CamerastatusInit;//""
			this.Cameractrl=_CameractrlNotAv;
			this.PlayerSmallIndex=_PlayerSmallHidden;
		}

		public function init(PlayerSmallVideoDisplay:VideoDisplay,equipdone:Function):void
		{
			_callback=equipdone;
			if (_avmic){
				_avmic.removeEventListener(StatusEvent.STATUS, Eventmicstatus);
				_avmic.removeEventListener(ActivityEvent.ACTIVITY,Eventmicactivity);
				_avmic=null;
				this.Micstatus=_MicstatusInit;//""
				this.Micctrl=_MicctrlNotAv;
			}
			_avmic=Microphone.getMicrophone(_micindex);
			log("[LocalEquip_init_micindex]"+_micindex+"\n");
			log("[LocalEquip_init_avmic]"+_avmic+"\n");
			if (_avmic)
			{
				_avmic.codec=SoundCodec.SPEEX;
				_avmic.setSilenceLevel(0);
				_avmic.framesPerPacket=1;
				_avmic.gain=_micvolume;
				_avmic.addEventListener(StatusEvent.STATUS, Eventmicstatus);
				_avmic.addEventListener(ActivityEvent.ACTIVITY, Eventmicactivity);
				this.Micstatus=_MicstatusWork;//"音频设备正常"
				this.Micctrl=_MicctrlEnable;
			}else{
				this.Micstatus=_MicstatusNotfound;//"没有找到音频设备"
				this.Micctrl=_MicctrlNotAv;
			}
			
			if( _avcamera ){
				_avcamera.removeEventListener(StatusEvent.STATUS, Eventcamerastatus);
				_avcamera.removeEventListener(ActivityEvent.ACTIVITY,Eventcameraactivity);
				_avcamera=null;
				this.Camerastatus=_CamerastatusInit;//""
				this.Cameractrl=_CameractrlNotAv;
				this.PlayerSmallIndex=_PlayerSmallHidden;
			}
			
			_avcamera=Camera.getCamera(_cameraindex.toString());
			log("[LocalEquip_init_cameraindex]"+_cameraindex.toString()+"\n");
			log("[LocalEquip_init_avcamera]"+_avcamera+"\n");
			if (_avcamera) {
				_avcamera.setMode(320, 240, 15); //320, 240 ,15
				_avcamera.setQuality(144000,85); //144000,85
//				_avcamera.setMode(160, 120, 15); //此设置需要 36KB/s 的带宽
//				_avcamera.setQuality(36000,0); // 每秒最大带宽为 36KB/s
				_avcamera.addEventListener(StatusEvent.STATUS, Eventcamerastatus);
				_avcamera.addEventListener(ActivityEvent.ACTIVITY, Eventcameraactivity);
				PlayerSmallVideoDisplay.attachCamera(null);
				PlayerSmallVideoDisplay.attachCamera(_avcamera);//执行这句时，flash才会弹出摄像头是否允许使用提示框
			}else{
				this.Camerastatus=_CamerastatusNotfound;//"没有找到摄像头"
				this.Cameractrl=_CameractrlNotAv;
				this.PlayerSmallIndex=_PlayerSmallNoVideoSelected;
			}
		}
		
		//用户选择"允许"或"拒绝"使用摄像头时触发
		private function Eventmicstatus(e:StatusEvent):void {
			FlexGlobals.topLevelApplication.log("[Eventmicstatus]"+e.code+"\n");
			if (e.code=="Microphone.Muted") {
				this.Micstatus=_MicstatusMuted;//"您未允许使用音频设备"
				this.Micctrl=_MicctrlNotAv;
			} else if (e.code == "Microphone.Unmuted") {
			}
		}
		//摄像头有活动时，被触发
		private function Eventmicactivity(e:ActivityEvent):void {
			//FlexGlobals.topLevelApplication.log("[Eventmicactivity]activity:"+e.activating+"\n");
		}
		
		//用户选择"允许"或"拒绝"使用摄像头时触发
		private function Eventcamerastatus(e:StatusEvent):void {
			FlexGlobals.topLevelApplication.log("[Eventcamerastatus]"+e.code+"\n");
			if (e.code=="Camera.Muted"){
				this.Camerastatus=_CamerastatusMuted;//"您未允许使用摄像头"
				this.Cameractrl=_CameractrlNotAv;
				this.PlayerSmallIndex=_PlayerSmallNoVideoSelected;
			} else if (e.code == "Camera.Unmuted") {
				this.Camerastatus=_CamerastatusUnmuted;//"摄像头视频采集中..."
				this.Cameractrl=_CameractrlNotAv;
				this.PlayerSmallIndex=_PlayerSmallNoVideoSelected;
				avchecktimer.start();
			}
		}
		//摄像头有活动时，被触发
		private function Eventcameraactivity(e:ActivityEvent):void {
			//FlexGlobals.topLevelApplication.log("[Eventcameraactivity]activity:"+e.activating+"\n");
		}
		private function avcheck(e:TimerEvent):void {
			log("[avcheck]currentFPS_" + _avcamera.currentFPS.toString()+"\n");
			if (_avcamera.currentFPS > 0){//视频设备可用
				avchecktimer.stop();
				this.Camerastatus=_CamerastatusWork;//"摄像头正常"
				this.Cameractrl=_CameractrlEnable;
				this.PlayerSmallIndex=_PlayerSmallVideoSelected;
				this.CameractrlEnable();
				_callback();
			}else{
				if (avchecktimer.currentCount>=AvCheckCount) {//持续检测AvCheckCount次(AvCheckCount*AvCheckCron秒)，仍然无图象的话，认为设备忙
					avchecktimer.stop();
					this.Camerastatus=_CamerastatusBusy;//"摄像头被占用"
					this.Cameractrl=_CameractrlNotAv;
					this.PlayerSmallIndex=_PlayerSmallNoVideoSelected;
				}
			}
		}
		
		public function CameractrlEnable():void
		{
			if(_Camerastatus==_CamerastatusWork){
				this.Cameractrl=_CameractrlEnable;
				this.PlayerOperationTalkingCameraIndex=_PlayerOperationTalkingCameraOnSelected;
			}else{
				this.Cameractrl=_CameractrlNotAv;
				this.PlayerOperationTalkingCameraIndex=_PlayerOperationTalkingCameraOffSelected;
			}
		}
		
		public function CameractrlDisable():void
		{
			if(_Camerastatus==_CamerastatusWork){
				this.Cameractrl=_CameractrlDisable;
			}else{
				this.Cameractrl=_CameractrlNotAv;
			}
			this.PlayerOperationTalkingCameraIndex=_PlayerOperationTalkingCameraOffSelected;
		}
		
		public function MicctrlEnable():void
		{
			if(_Micstatus==_MicstatusWork){
				this.Micctrl=_MicctrlEnable;
				this.PlayerOperationTalkingMicIndex=_PlayerOperationTalkingMicOnSelected;
			}else{
				this.Micctrl=_MicctrlNotAv;
				this.PlayerOperationTalkingMicIndex=_PlayerOperationTalkingMicOffSelected;
			}
		}
		
		public function MicctrlDisable():void
		{
			if(_Micstatus==_MicstatusWork){
				this.Micctrl=_MicctrlDisable;
			}else{
				this.Micctrl=_MicctrlNotAv;
			}
			this.PlayerOperationTalkingMicIndex=_PlayerOperationTalkingMicOffSelected;
		}
		
		public function get Micstatus():String
		{
			return _Micstatus;
		}
		
		public function get MicstatusWork():String
		{
			return _MicstatusWork;
		}
		
		public function get Camerastatus():String
		{
			return _Camerastatus;
		}
		
		public function get CamerastatusWork():String
		{
			return _CamerastatusWork;
		}

		public function get Micctrl():String
		{
			return _Micctrl;
		}
		
		public function get Cameractrl():String
		{
			return _Cameractrl;
		}
		
		public function get PlayerSmallIndex():int
		{
			return _PlayerSmallIndex;
		}
		
		public function get PlayerSmallHidden():int
		{
			return _PlayerSmallHidden;
		}
		
		[Bindable]
		public function set Micstatus(value:String):void
		{
			_Micstatus = value;
		}
		
		[Bindable]
		public function set Camerastatus(value:String):void
		{
			_Camerastatus = value;
		}
		
		[Bindable]
		public function set Micctrl(value:String):void
		{
			_Micctrl = value;
		}
		
		[Bindable]
		public function set Cameractrl(value:String):void
		{
			_Cameractrl = value;
		}
		
		[Bindable]
		public function set PlayerSmallIndex(value:int):void
		{
			_PlayerSmallIndex = value;
		}
		
		public function get PlayerOperationTalkingCameraIndex():int
		{
			return _PlayerOperationTalkingCameraIndex;
		}
		
		[Bindable]
		public function set PlayerOperationTalkingCameraIndex(value:int):void
		{
			_PlayerOperationTalkingCameraIndex = value;
		}
		
		public function get PlayerOperationTalkingMicIndex():int
		{
			return _PlayerOperationTalkingMicIndex;
		}
		
		[Bindable]
		public function set PlayerOperationTalkingMicIndex(value:int):void
		{
			_PlayerOperationTalkingMicIndex = value;
		}
		
		public function setrates(recvInfo:NetStreamInfo,sentInfo:NetStreamInfo):void
		{
			//删除数组中第一个元素,其余数组元素将从其原始位置 i 移至 i-1
			audioRate.shift(); 
			var a:Object=new Object;
			a.Recv=recvInfo.audioBytesPerSecond * 8 / 1024;
			a.Sent=sentInfo.audioBytesPerSecond * 8 / 1024;
			audioRate.push(a);
			audioRateDisplay.source=audioRate;
			
			videoRate.shift();
			var v:Object=new Object;
			v.Recv=recvInfo.videoBytesPerSecond * 8 / 1024;
			v.Sent=sentInfo.videoBytesPerSecond * 8 / 1024;
			videoRate.push(v);
			videoRateDisplay.source=videoRate;
			
			srtt.shift();
			var s:Object=new Object;
			s.Data=recvInfo.SRTT;
			srtt.push(s);
			srttDisplay.source=srtt;
		}
		
		public function get avmic():Microphone
		{
			return _avmic;
		}
		
		public function get avcamera():Camera
		{
			return _avcamera;
		}
		
		public function getmic(index:int):Microphone
		{
			return Microphone.getMicrophone(index);
		}
		
		public function getcamera(index:int):Camera
		{
			return Camera.getCamera(index.toString());
		}
		
		public function get micindex():int
		{
			return _micindex;
		}
		
		public function get cameraindex():int
		{
			return _cameraindex;
		}

		public function set micindex(value:int):void
		{
			_micindex = value;
		}

		public function get micvolume():int
		{
			return _micvolume;
		}

		public function set micvolume(value:int):void
		{
			_micvolume = value;
			_avmic.gain=_micvolume; //麦克风放大信号的程度
			_localSO.data.micVolume=value;
			_localSO.flush();
		}

		public function get speakervolume():Number
		{
			return _speakervolume;
		}

		public function set speakervolume(value:Number):void
		{
			_speakervolume = value;
			_localSO.data.speakerVolume=value;
			_localSO.flush(); //将本地永久共享对象立即写入本地文件
		}
		private function log(msg:String):void{
			FlexGlobals.topLevelApplication.log(msg);
		}

	}
}