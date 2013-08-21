package com.renren.ui
{
	import mx.controls.Image;
	import flash.events.MouseEvent;
	
	[Style(name="borderColor", type="uint", format="Color", inherit="no")]
	[Style(name="borderWidth", type="Number", format="Length", inherit="no")]
	[Style(name="borderAlpha", type="Number", format="Length", inherit="no")]
	
	public class Photo extends Image
	{
		public function Photo()
		{
			super();
		}
		
		override protected function updateDisplayList(w:Number, h:Number):void
		{
			super.updateDisplayList(w,h);
			if(getStyle('borderWidth')==undefined)
			{
				setStyle('borderWidth','0');
			}
			if(getStyle('borderColor')==undefined)
			{
				setStyle('borderColor','');
			}
			if(getStyle('borderAlpha')==undefined)
			{
				setStyle('borderAlpha','0.0');
			}
			graphics.clear();
			graphics.lineStyle(Number(getStyle('borderWidth')),getStyle('borderColor'),Number(getStyle('borderAlpha')),false);
			var x:Number=-(getStyle('borderWidth')/2);
			var y:Number=-(getStyle('borderWidth')/2);
			var width:Number=this.width+Number(getStyle('borderWidth'));
			var height:Number=this.height+Number(getStyle('borderWidth'));
			graphics.drawRect(x,y,width,height);
		}
	} 
}