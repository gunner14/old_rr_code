package com.renren.module
{
	import com.renren.net.webserver.ProtocalTimer;
	
	import flash.events.TimerEvent;
	import flash.net.NetStream;
	import flash.utils.Timer;
	
	import mx.formatters.DateFormatter;

	public class LocalStatus {
		
		private var _istalking:Boolean;
		
		private var _State:String;
		private const _StateIniting:String="Initing";		//正在初始化
		private const _StateReady:String="Ready";						//安静态
		private const _StateConnecting:String="Connecting";			//正在连接,此状态在正在呼出和正在呼入前都会发生
		private const _StateCalling:String="Calling";				//正在呼出
		private const _StateRinging:String="Ringing";				//正在呼入
		private const _StateTalking:String="Talking";				//正在通话
		
		private var _PlayerBigIndex:int;						//大视频区
		private const _PlayerBigVideoSelected:int = 0;					//已连接并对方有视频时显示
		private const _PlayerBigOutgoingSelected:int = 1;				//已连接并对方无视频时显示
		private const _PlayerBigIncomingSelected:int = 2;				//已连接并对方无视频时显示
		private const _PlayerBigHidden:int = 3;						//隐藏
		
		private var _PlayerNoteIndex:int;						//提示区
		private const _PlayerNoteStatusSelected:int=0;					//文字提示区
		private const _PlayerNoteIncomingSelected:int=1;				//正在呼入
		private const _PlayerNoteOutgoingSelected:int=2;				//正在呼出
		private const _PlayerNoteResetSelected:int=3;					//询问是否重设
		private const _PlayerNoteControlSelected:int=4;					//通话中的设备状态
		private const _PlayerNoteHidden:int = 5;						//隐藏
		
		private var _PlayerNoteStatusMsg:String;
		
		private var _PlayerOperationIndex:int;					//用户操作区
		private const _PlayerOperationTalkingSelected:int = 0;			//通话时显示
		private const _PlayerOperationHangingSelected:int = 1;			//挂断时显示
		private const _PlayerOperationHidden:int = 2;					//隐藏
		
		private var _PlayerDebugIndex:int;						//调试区
		private const _PlayerDebugLogSelected:int = 0;					//Log
		private const _PlayerDebugImSelected:int = 1;					//文字通话
		private const _PlayerDebugFeedbackSelected:int = 2;			//用户反馈
		private const _PlayerDebugHidden:int = 3;						//隐藏
		
		private var _incomingitem:IncomingItem;				//呼入管理器
		private var _outgoingitem:OutgoingItem;				//呼出管理器
		private var _protocaltimer:ProtocalTimer;				//定时连接器
		private var _hostiscaller:Boolean;						//通话中的身份
		private var _incomingstream:NetStream;				//输入流
		
		private var _listingtimer:Timer;						//来电查询定时器
		private const _listingcron:int = 5;						//来电查询定时间隔
		
		private var _holdingtimer:Timer;						//呼出定时器
		private const _holdingcron:int = 2;						//呼出定时间隔
		private const _holdingdeadline:int = 120;					//呼出无应答期限
		private var _holdingtime:int;								//呼出计时
		private const _holdingtimeout:int = 5;						//延续呼叫的秒数
		
		private var _ringingtimer:Timer;						//呼入定时器
		private const _ringingcron:int = 5;						//呼入定时间隔
		
		private var _talkingtimer:Timer;						//通话定时时器
		private const _talkingcron:int = 1;						//通话定时间隔
		private var _talkingtimesec:int;							//通话计时(秒)
		private var _talkingtime:String;							//通话计时(00:00:00)
		private const _talkingpostcron:int = 15;					//定时汇报间隔
		private const _talkingtimeout:int = 60;					//服务器维持时间
		private const _talkingqualitycron:int = 5;					//定时质检间隔
		private var _talkingquality:String;						//质检
		
		private const _RoleFree:int=0;
		private const _RoleCaller:int=1;
		private const _RoleReciever:int=2;
		private var _Role:int;
		
		private var _initingremoteid:String;
		private var _refreshingremoteid:String;
		private var _refreshingauto:Boolean;
		
		public function LocalStatus(incomingitem:IncomingItem,outgoingitem:OutgoingItem,protocaltimer:ProtocalTimer){
			_incomingitem=incomingitem;
			_outgoingitem=outgoingitem;
			
			_protocaltimer=protocaltimer;
			
			_Role=_RoleFree;
			
			_listingtimer = new Timer(1000 * _listingcron);
			_listingtimer.addEventListener(TimerEvent.TIMER, Eventlistingtimer);
			
			_holdingtimer = new Timer(1000 * _holdingcron);
			_holdingtimer.addEventListener(TimerEvent.TIMER, Eventholdingtimer);
			
			_ringingtimer = new Timer(1000 * _ringingcron);
			_ringingtimer.addEventListener(TimerEvent.TIMER, Eventringingtimer);
			
			_talkingtimer = new Timer(1000 * _talkingcron);
			_talkingtimer.addEventListener(TimerEvent.TIMER, Eventtalkingtimer);
			
			this.holdingtime=0;
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=false;
			_incomingstream=null;
		}
		
		public function get holdingtimeout():int
		{
			return _holdingtimeout;
		}

		public function init(msg:String,initingremoteid:String):void{
			_Role=_RoleFree;
			
			this.State=_StateIniting;
			this.PlayerBigIndex=_PlayerBigHidden;
			this.PlayerNoteIndex=_PlayerNoteHidden;
			this.PlayerOperationIndex=_PlayerOperationHidden;
			this.PlayerDebugIndex=_PlayerDebugHidden;
			
			this.PlayerNoteStatusMsg=msg;
			
			_listingtimer.stop();
			_holdingtimer.stop();
			_ringingtimer.stop();
			_talkingtimer.stop();
			
			_initingremoteid=initingremoteid;
			
			this.holdingtime=0;
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=false;
			_incomingstream=null;
		}
		
		public function ready(showstatus:Boolean,msg:String,listing:Boolean):void{
			_Role=_RoleFree;
			
			this.State=_StateReady;
			this.PlayerBigIndex=_PlayerBigHidden;
			if( showstatus ){
				this.PlayerNoteStatusMsg=msg;
				this.PlayerNoteIndex=_PlayerNoteStatusSelected;
			}else{
				this.PlayerNoteStatusMsg="";
				this.PlayerNoteIndex=_PlayerNoteHidden;
			}
			this.PlayerOperationIndex=_PlayerOperationHidden;
			//this.PlayerDebugIndex=_PlayerDebugHidden;
			
			if(listing){
				_listingtimer.start();
//				if(_protocaltimer)_protocaltimer.listing(Math.random().toString());
			}else{
				_listingtimer.stop();
			}
			_holdingtimer.stop();
			_ringingtimer.stop();
			_talkingtimer.stop();
			
			this.holdingtime=0;
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=false;
			_incomingstream=null;
		}
		
		public function connect(iscaller:Boolean,msg:String):void{
			if( iscaller ){
				_Role=_RoleCaller;
			}else{
				_Role=_RoleReciever;
			}
			
			this.State=_StateConnecting;
			this.PlayerBigIndex=_PlayerBigHidden;
			this.PlayerNoteStatusMsg=msg;
			this.PlayerNoteIndex=_PlayerNoteStatusSelected;
			this.PlayerOperationIndex=_PlayerOperationHidden;
			//this.PlayerDebugIndex=_PlayerDebugHidden;
			
			
			_listingtimer.stop();
			_holdingtimer.stop();
			_ringingtimer.stop();
			_talkingtimer.stop();
			
			this.holdingtime=0;
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=false;
			_incomingstream=null;
		}
		
		public function call():void{
			_Role=_RoleCaller;
			
			this.State=_StateCalling;
			this.PlayerBigIndex=_PlayerBigHidden;
			this.PlayerNoteIndex=_PlayerNoteOutgoingSelected;
			this.PlayerOperationIndex=_PlayerOperationHidden;
			//this.PlayerDebugIndex=_PlayerDebugHidden;
			
			this.holdingtime=0;
			
			_listingtimer.stop();
			_holdingtimer.start();
			_ringingtimer.stop();
			_talkingtimer.stop();
			
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=false;
			_incomingstream=null;
		}
		
		public function ring():void{
			_Role=_RoleReciever;
			
			this.State=_StateRinging;
			this.PlayerBigIndex=_PlayerBigHidden;
			this.PlayerNoteIndex=_PlayerNoteIncomingSelected;
			this.PlayerOperationIndex=_PlayerOperationHidden;
			//this.PlayerDebugIndex=_PlayerDebugHidden;
			
			_listingtimer.stop();
			_holdingtimer.stop();
			_ringingtimer.start();
			_talkingtimer.stop();
			
			this.holdingtime=0;
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=false;
			_incomingstream=null;
		}
		
		public function talk(incomingstream:NetStream,iscaller:Boolean):void{
			this.State=_StateTalking;
			if( iscaller ){
				_Role=_RoleCaller;
				this.PlayerBigIndex=_PlayerBigOutgoingSelected;
			}else{
				_Role=_RoleReciever;
				this.PlayerBigIndex=_PlayerBigIncomingSelected;
			}
			this.PlayerNoteIndex=_PlayerNoteControlSelected;
			this.PlayerOperationIndex=_PlayerOperationTalkingSelected;
			//this.PlayerDebugIndex=_PlayerDebugHidden;
			
			_talkingtimesec=0;
			_talkingquality="";
			this.talkingtime="";
			this.istalking=true;
			_hostiscaller=iscaller;
			_incomingstream=incomingstream;
			
			_listingtimer.stop();
			_holdingtimer.stop();
			_ringingtimer.stop();
			_talkingtimer.start();
			
			this.holdingtime=0;
		}
		
		public function refresh(refreshingremoteid:String):void{
			this._refreshingremoteid=refreshingremoteid;
			this.PlayerNoteIndex=_PlayerNoteResetSelected;
		}
		
		public function readystatus(msg:String):void{
			if( _State==_StateReady){
				this.PlayerNoteIndex=_PlayerNoteStatusSelected;
			}
		}
		
		public function ringpause():void{
			if( _State==_StateRinging){
				this.PlayerNoteIndex=_PlayerNoteHidden;
			}
		}
		
		private function Eventlistingtimer(e:Event):void
		{
			if(_protocaltimer)_protocaltimer.listing(Math.random().toString());
		}
		
		private function Eventholdingtimer(e:Event):void
		{
			if(_protocaltimer){
				this.holdingtime+=_holdingcron;
				_protocaltimer.holding(Math.random().toString(),_holdingtimeout);
			}
		}
		
		private function Eventringingtimer(e:Event):void
		{
			if(_protocaltimer)_protocaltimer.ringing(_incomingitem.userid);
		}
		
		private function Eventtalkingtimer(e:TimerEvent):void
		{
			_talkingtimesec++;
			var elapsed:Date=new Date(2008,4,12);
			elapsed.setTime(elapsed.getTime()+_talkingtimesec*1000);
			var formatter:DateFormatter=new DateFormatter();
			var format:String="JJ:NN:SS";
			formatter.formatString=format;
			this.talkingtime=formatter.format(elapsed);
			
			//			if (State == StateTalking)// && xnIncomingStream && xnOutgoingStream && xnOutgoingStream.peerStreams.length == 1)
			//			{
			//				var recvInfo:NetStreamInfo;
			//				var sentInfo:NetStreamInfo;
			//				if( VideoConnectRtmfpOk ){
			//					recvInfo=xnrtmfpconnectmanager.getinstreaminfo();
			//					sentInfo=xnrtmfpconnectmanager.getoutstreaminfo();
			//				}else if( VideoConnectRtmpOk ){
			//					recvInfo=xnrtmpconnectmanager.getinstreaminfo();
			//					sentInfo=xnrtmpconnectmanager.getoutstreaminfo();
			//				}
			//				if( recvInfo && sentInfo ){
			//					xnlocalequip.setrates(recvInfo,sentInfo);
			//				}
			//			}
			
			if( _incomingstream==null ){
				this.PlayerBigIndex=_PlayerBigHidden;
			}else{
				if( _talkingtimesec%_talkingqualitycron == 0 ){
					_talkingquality=_talkingquality+"_"+_incomingstream.currentFPS.toFixed(2);
				}
				if(_incomingstream.currentFPS==0){
					if( _hostiscaller ){
						this.PlayerBigIndex=_PlayerBigOutgoingSelected;
					}else{
						this.PlayerBigIndex=_PlayerBigIncomingSelected;
					}
				}else{//(incomingstream.currentFPS>0)
					this.PlayerBigIndex=_PlayerBigVideoSelected;
				}
			}
			
			if( _talkingtimesec%_talkingpostcron == 0 && _protocaltimer && _incomingstream ){
				if(_hostiscaller){
					_protocaltimer.talking(_outgoingitem.userid,"true",_talkingquality,talkingtime,String(_talkingtimeout));
				}else{
					_protocaltimer.talking(_incomingitem.userid,"false",_talkingquality,talkingtime,String(_talkingtimeout));
				}
				_talkingquality="";
			}
		}
		
		public function playerin():void
		{
			if( _istalking && _PlayerNoteIndex!=_PlayerNoteResetSelected ){
				this.PlayerNoteIndex=_PlayerNoteControlSelected;
				this.PlayerOperationIndex=_PlayerOperationTalkingSelected;
			}
		}
		
		public function playerout():void
		{
			if( _istalking && _PlayerNoteIndex!=_PlayerNoteResetSelected ){
				this.PlayerNoteIndex=_PlayerNoteHidden;
				this.PlayerOperationIndex=_PlayerOperationHidden;
			}else if(_State==_StateReady){
				this.PlayerNoteIndex=_PlayerNoteHidden;
			}
		}
		
		public function get State():String
		{
			return _State;
		}
		
		[Bindable]
		public function set State(value:String):void
		{
			_State = value;
		}

		public function get PlayerBigIndex():int
		{
			return _PlayerBigIndex;
		}
		
		[Bindable]
		public function set PlayerBigIndex(value:int):void
		{
			_PlayerBigIndex = value;
		}

		public function get PlayerNoteIndex():int
		{
			return _PlayerNoteIndex;
		}
		
		[Bindable]
		public function set PlayerNoteIndex(value:int):void
		{
			_PlayerNoteIndex = value;
		}

		public function get PlayerOperationIndex():int
		{
			return _PlayerOperationIndex;
		}
		
		[Bindable]
		public function set PlayerOperationIndex(value:int):void
		{
			_PlayerOperationIndex = value;
		}

		public function get PlayerDebugIndex():int
		{
			return _PlayerDebugIndex;
		}
		
		[Bindable]
		public function set PlayerDebugIndex(value:int):void
		{
			_PlayerDebugIndex = value;
		}

		public function get talkingtime():String
		{
			return _talkingtime;
		}
		
		[Bindable]
		public function set talkingtime(value:String):void
		{
			_talkingtime = value;
		}
		
		public function get talkingtimeout():int
		{
			return _talkingtimeout;
		}

		public function get holdingtime():int
		{
			return _holdingtime;
		}
		
		[Bindable]
		public function set holdingtime(value:int):void
		{
			_holdingtime = value;
		}

		public function get holdingdeadline():int
		{
			return _holdingdeadline;
		}
		
		[Bindable]
		public function set istalking(value:Boolean):void
		{
			_istalking = value;
		}
		
		public function isready():Boolean
		{
			return _State==_StateReady
		}
		
//		public function iscalling():Boolean
//		{
//			return _State==_StateCalling;
//		}
		
//		public function isconnecting():Boolean
//		{
//			return _State==_StateConnecting;
//		}
		
		public function get istalking():Boolean
		{
			return _istalking;
		}
		
		public function iscaller():Boolean
		{
			return _Role==_RoleCaller;
		}
		
		public function isreciever():Boolean
		{
			return _Role==_RoleReciever;
		}

		public function get PlayerBigHidden():int
		{
			return _PlayerBigHidden;
		}

		public function get PlayerNoteHidden():int
		{
			return _PlayerNoteHidden;
		}

		public function get PlayerOperationHidden():int
		{
			return _PlayerOperationHidden;
		}

		public function get PlayerDebugHidden():int
		{
			return _PlayerDebugHidden;
		}

		public function get PlayerNoteStatusMsg():String
		{
			return _PlayerNoteStatusMsg;
		}
		
		[Bindable]
		public function set PlayerNoteStatusMsg(value:String):void
		{
			_PlayerNoteStatusMsg = value;
		}

		public function poprefreshingid():String
		{
			var tmp:String = _refreshingremoteid;
			_refreshingremoteid = "-1";
			return tmp;
		}

		public function popinitingid():String
		{
			var res:String = _initingremoteid;
			_initingremoteid = "-1";
			return res;
		}
		
		public function get refreshingauto():Boolean
		{
			return _refreshingauto;
		}
		
		public function set refreshingauto(value:Boolean):void
		{
			_refreshingauto = value;
		}


	}
}