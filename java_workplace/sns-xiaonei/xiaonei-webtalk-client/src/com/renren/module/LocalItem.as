package com.renren.module
{
	import mx.binding.utils.BindingUtils;

	public class LocalItem {
		private var _userid:String;
		private var _username:String;
		private var _tinyurl:String;
		private var _mainurl:String;
		private var _weburl:String;
		private var _rtmfpurl:String;
		private var _rtmpurl:String;
		
		public function LocalItem(
			userid:String,
			username:String,
			tinyurl:String,
			mainurl:String,
			weburl:String,
			rtmfpurl:String,
			rtmpurl:String
		){
			this._userid=userid;
			this._username=username;
			this._tinyurl=tinyurl;
			this._mainurl=mainurl;
			this._weburl=weburl;
			this._rtmfpurl=rtmfpurl;
			this._rtmpurl=rtmpurl;
		}
		public function get rtmpurl():String
		{
			return _rtmpurl;
		}

		public function get rtmfpurl():String
		{
			return _rtmfpurl;
		}

		public function weburl(path:String):String
		{
			return _weburl+"/"+path;
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
		
		[Bindable]
		public function set userid(value:String):void
		{
			_userid = value;
		}

		[Bindable]
		public function set username(value:String):void
		{
			_username = value;
		}

		[Bindable]
		public function set tinyurl(value:String):void
		{
			_tinyurl = value;
		}

		[Bindable]
		public function set mainurl(value:String):void
		{
			_mainurl = value;
		}


	}
}