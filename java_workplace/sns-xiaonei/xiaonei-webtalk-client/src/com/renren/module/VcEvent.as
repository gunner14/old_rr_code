package com.renren.module
{
	import flash.events.Event;

	public class VcEvent extends Event
	{
		public function VcEvent(type:String)
		{
			super(type);
		}
		override public function clone():Event
		{
			return new VcEvent(type);
		}
	}
}