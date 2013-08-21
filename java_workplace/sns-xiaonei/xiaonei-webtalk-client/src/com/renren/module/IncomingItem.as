package com.renren.module
{
	public class IncomingItem
	{
		private var _userid:String;
		private var _username:String;
		private var _tinyurl:String;
		private var _mainurl:String;
		private var _peer:String;
		private var _rtmfpok:int;
		private var _rtmpok:int;
		private var _rtmfpurl:String;
		private var _rtmpurl:String;
		
		public function IncomingItem(
			userid:String,
			username:String,
			tinyurl:String,
			mainurl:String,
			peer:String,
			rtmfpok:int,
			rtmpok:int,
			rtmfpurl:String,
			rtmpurl:String
		){
			this._userid=userid;
			this._username=username;
			this._tinyurl=tinyurl;
			this._mainurl=mainurl;
			this._peer=peer;
			this._rtmfpok=rtmfpok;
			this._rtmpok=rtmpok;
			this._rtmfpurl=rtmfpurl;
			this._rtmpurl=rtmpurl;
		}
		
		public function set rtmpurl(value:String):void
		{
			_rtmpurl = value;
		}

		public function set rtmfpurl(value:String):void
		{
			_rtmfpurl = value;
		}

		public function set rtmpok(value:int):void
		{
			_rtmpok = value;
		}

		public function set rtmfpok(value:int):void
		{
			_rtmfpok = value;
		}

		public function set peer(value:String):void
		{
			_peer = value;
		}
		
		[Bindable]
		public function set mainurl(value:String):void
		{
			_mainurl = value;
		}
		
		[Bindable]
		public function set tinyurl(value:String):void
		{
			_tinyurl = value;
		}
		
		[Bindable]
		public function set username(value:String):void
		{
			_username = value;
		}
		
		[Bindable]
		public function set userid(value:String):void
		{
			_userid = value;
		}

		public function get rtmpurl():String
		{
			return _rtmpurl;
		}
		
		public function get rtmfpurl():String
		{
			return _rtmfpurl;
		}
		
		public function get rtmpok():int
		{
			return _rtmpok;
		}
		
		public function get rtmfpok():int
		{
			return _rtmfpok;
		}
		
		public function get peer():String
		{
			return _peer;
		}
		
		public function get mainurl():String
		{
			return _mainurl;
		}
		
		public function get tinyurl():String
		{
			return _tinyurl;
		}
		
		public function get username():String
		{
			return _username;
		}
		
		public function get userid():String
		{
			return _userid;
		}
		
	}
}