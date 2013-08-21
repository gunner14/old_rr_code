package com.renren.module
{
	import mx.core.FlexGlobals;

	public class OutgoingItem
	{
		private var _userid:String;
		private var _username:String;
		private var _tinyurl:String;
		private var _mainurl:String;
		
		private var _refreshing:Boolean;
		
		public function OutgoingItem(
			userid:String,
			username:String,
			tinyurl:String,
			mainurl:String
		){
			this.userid=userid;
			this.username=username;
			this.tinyurl=tinyurl;
			this.mainurl=mainurl;
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