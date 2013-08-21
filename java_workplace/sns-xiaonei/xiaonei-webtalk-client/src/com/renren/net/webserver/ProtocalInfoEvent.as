package com.renren.net.webserver
{
	import flash.events.Event;

	public class ProtocalInfoEvent extends Event
	{
		public var userid:String;
		public var username:String;
		public var tinyurl:String;
		public var mainurl:String;
		public var status:String;
		public var permission:String;
		
		public function ProtocalInfoEvent(
			type:String,
			userid:String,
			username:String,
			tinyurl:String,
			mainurl:String,
			status:String,
			permission:String
		)
		{
			super(type);
			this.userid = userid;
			this.username = username;
			this.tinyurl = tinyurl;
			this.mainurl = mainurl;
			this.status = status;
			this.permission = permission;
		}
		override public function clone():Event
		{
			return new ProtocalInfoEvent(type, userid, username, tinyurl, mainurl, status, permission);
		}
	}
}