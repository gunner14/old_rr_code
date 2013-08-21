package com.renren.net.webserver
{
	import flash.events.Event;

	public class ProtocalListEvent extends Event
	{
		public var users:Array;
		
		public function ProtocalListEvent(type:String, users:Array)
		{
			super(type);
			this.users = users;
		}
		override public function clone():Event
		{
			return new ProtocalListEvent(type, users);
		}
	}
}