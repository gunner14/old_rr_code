package com.renren.module
{
	import com.renren.net.webserver.ProtocalAdapter;
	
	public class BwCheck
	{
		private var S_C_KB :int = 0;
		private var C_S_KB :int = 0;
		private var bwok :Boolean = false;
		private var actions:Array = new Array();
		private var protocaladapter:ProtocalAdapter = null;
		private var localitem:LocalItem = null;
		
		public function BwCheck(protocaladapter:ProtocalAdapter,localitem:LocalItem)
		{
			this.protocaladapter = protocaladapter;
			this.localitem = localitem;
		}
		public function setAction(crid:String,action:String,rtfmpStage:int,rtmpStage:int,url:String):void
		{
			var action:String = "crid:"+crid+" action:"+action+" rtmfpStage:"+rtfmpStage+" rtmpStage:"+rtmpStage+" url:"+url;
			if(bwok){
				sendAction(action);
			}else{
				actions.push(action);
			}
			
		}
		public function setSCBW(S_C_KB:int):void
		{
			this.S_C_KB = S_C_KB;
		}
		public function setCSBW(C_S_KB:int):void
		{
			this.C_S_KB = C_S_KB;
			bwok = true;
			for each( var action:String in actions ){
				sendAction(action);
			}
			
		}
		private function sendAction(action:String):void
		{
			protocaladapter.bwcheck(localitem.weburl("bwcheck"),action,S_C_KB,C_S_KB);
				
		}
	}
}