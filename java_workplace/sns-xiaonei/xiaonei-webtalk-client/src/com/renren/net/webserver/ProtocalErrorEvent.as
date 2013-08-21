package com.renren.net.webserver
{
	import flash.events.Event;
	
	public class ProtocalErrorEvent extends Event
	{
		public var description:String;
		
		public function ProtocalErrorEvent(type:String, desc:String)
		{
			super(type);
			this.description = desc;
		}
		
		override public function clone():Event
		{
			return new ProtocalErrorEvent(type, description);
		}
	}
}