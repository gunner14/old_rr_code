package com.renren.module
{
	import com.renren.net.webserver.ProtocalInfoEvent;
	
	import flash.events.TimerEvent;
	import flash.utils.Dictionary;
	import flash.utils.Timer;

	public class Lotto
	{
		private var lottoids:Array = [];
		private var lottoindex:int=0;
		private const lottocron:int = 50;
		private var lottotimer:Timer = new Timer(lottocron);
		
		[Bindable] public var lottoitem:LocalItem = new LocalItem("0","会是谁呢","","","0","0","0");
		
		private var _PlayerDebugVoteIndex:int=0;
		
		private var lottodone:Boolean=false;
		
		public function Lotto(){
			
		}
		private var lottodoneids:Dictionary = new Dictionary();
		
		private var _lottoohyeah:Boolean=false;
		
		
		public function lottoinit():void{
			lottoitem.userid = "0";
			lottoitem.username = "会是谁呢";
			lottoitem.tinyurl = "";
			if( lottoids.length>0 ){
				lottotimer.addEventListener(TimerEvent.TIMER,Eventlottotimer);
				//				if( xnlocalitem ){
				//					xnconnectmanager.addEventListener("LottoSucc", Eventlotto);
				//				}
				this.PlayerDebugVoteIndex = 1;
			}else{
				this.lottodone=true;
				this.PlayerDebugVoteIndex = 3;
			}
		}
		public function lottostart():void{
			if( !lottodone ){
				lottoitem.userid = "0";
				lottoitem.username = "会是谁呢";
				lottoitem.tinyurl = "";
				this.PlayerDebugVoteIndex = 2;
				lottotimer.start();
				this.lottoohyeah=false;
			}
		}
		public function lottostop():void{
			lottotimer.stop();
			lottodoneids[lottoindex]=lottoids[lottoindex];
			this.lottoohyeah=true;
			this.PlayerDebugVoteIndex = 1;
//			if( xnlocalitem ){
//				xnconnectmanager.lotto(xnlocalitem.weburl("info"), lottoitem.userid);
//			}
		}
		private function Eventlotto(e:Event):void
		{
			var event:ProtocalInfoEvent=e as ProtocalInfoEvent;
			lottoitem.userid = event.userid;
			lottoitem.username = event.username;
			lottoitem.tinyurl = event.tinyurl;
		}
		private function Eventlottotimer(e:TimerEvent):void
		{
			lottoindex=(lottoindex+1)%lottoids.length;
			var oldindex:int=lottoindex;
			while(true){
				if( lottodoneids[lottoindex] ){
					lottoindex=(lottoindex+1)%lottoids.length;
					if( lottoindex==oldindex ){
						lottodone=true;
						lottotimer.stop();
						PlayerDebugVoteIndex = 3;
						break;
					}
				}else{
					break;
				}
			}
			if( !lottodone ){
				lottoitem.userid = lottoids[lottoindex];
			}
		}

		public function get PlayerDebugVoteIndex():int
		{
			return _PlayerDebugVoteIndex;
		}
		
		[Bindable]
		public function set PlayerDebugVoteIndex(value:int):void
		{
			_PlayerDebugVoteIndex = value;
		}

		public function get lottoohyeah():Boolean
		{
			return _lottoohyeah;
		}
		
		[Bindable] 
		public function set lottoohyeah(value:Boolean):void
		{
			_lottoohyeah = value;
		}

		
	}
}