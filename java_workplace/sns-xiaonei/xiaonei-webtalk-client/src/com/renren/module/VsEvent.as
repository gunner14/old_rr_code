package com.renren.module
{
	import flash.events.Event;

	public class VsEvent extends Event
	{
		public function VsEvent(type:String)
		{
			super(type);
		}
		override public function clone():Event
		{
			return new VsEvent(type);
		}
	}
}