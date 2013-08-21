package com.renren.net.videoserver
{
	import flash.events.Event;

	public class VideoConnectEvent extends Event
	{
		public var conn_fail_func:Function;
		
		public function VideoConnectEvent(type:String, connectFaildFunc:Function)
		{
			super(type);
			this.conn_fail_func = connectFaildFunc;
		}
		override public function clone():Event
		{
			return new VideoConnectEvent(type, conn_fail_func);
		}
	}
}