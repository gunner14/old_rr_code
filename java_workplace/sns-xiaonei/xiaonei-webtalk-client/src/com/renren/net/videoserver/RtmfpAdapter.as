package com.renren.net.videoserver
{
	import com.renren.module.LocalItem;
	import com.renren.module.LocalStatus;
	import com.renren.module.Vctrl;
	import com.renren.net.videoserver.VideoAdapter;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.NetStatusEvent;
	import flash.media.Camera;
	import flash.media.Microphone;
	import flash.media.SoundTransform;
	import flash.net.NetConnection;
	import flash.net.NetStream;
	import flash.net.NetStreamInfo;
	
	import mx.core.FlexGlobals;
	
	public class RtmfpAdapter extends VideoAdapter
	{
		/**
		 * rtmfp vc连接事件
		 */
		[Event(name="vc_rtmfp", type="com.renren.net.videoserver.VideoConnectEvent")]
		
		private var _farID:String = "";
		
		public function RtmfpAdapter(vctrl:Vctrl,localstatus:LocalStatus,localitem:LocalItem)
		{
		    super(vctrl,localstatus,localitem);
			this.addEventListener("vc_rtmfp", this.vc_event_handler);
		}
		
		public function get farID():String
		{
			return _farID;
		}

		public function set farID(value:String):void
		{
			_farID = value;
		}

		public function getCumulusServer():String
		{
			var cumulusServer:String = "";
			cumulusServer = videourl.search("webtalk1")!=-1?"44":"45";
			return cumulusServer;
		}
		override protected function vc_event_handler(e:Event):void
		{
			var event:VideoConnectEvent = e as VideoConnectEvent;
			try
			{ 
				_conn.connect(videourl);
			}
			catch (e:ArgumentError)
			{
				_conn.close();
				_conn = null;
				event.conn_fail_func();
			}
		}
		
		public function getrtmfppeer():String
		{
			if( _open && _conn ){
				return _conn.nearID;
			}
			return "0";
		}
		
		override public function vc():void
		{
			if( !_open || _conn ){
				vctrl.vc_rtmfp_fail();
			}
			
			_conn = new NetConnection();
			_conn.addEventListener(NetStatusEvent.NET_STATUS, event_rtmfp_vc_vs);
			
			var d:VideoConnectEvent = new VideoConnectEvent("vc_rtmfp",vctrl.vc_rtmfp_fail);
			dispatchEvent(d);
			
			vctrl.vc_timer_start();
		}
		/**
		 * 处理 NetStatusEvent 事件，根据连接的结果进行不同函数的调用
		 */
		private function event_rtmfp_vc_vs(event:NetStatusEvent):void
		{
			log("[event_rtmfp_vc_vs]handler_"+event.info.code+"\n");
			switch (event.info.code)
			{
				case "NetConnection.Connect.Success":
					vctrl.vc_rtmfp_succ(getrtmfppeer());
					break;
				
				case "NetConnection.Connect.Closed":
					vctrl.vc_rtmfp_close();
					vctrl.vc_closed_rtmfpconn(localstatus);
					break;
				
				case "NetStream.Connect.Success":
					vctrl.vs_succ_rtmfp(event.info.stream.farID);
					break;
				
				case "NetConnection.Connect.Failed":
					vctrl.vc_rtmfp_fail();
					break;
				
				case "NetStream.Connect.Closed":
					vctrl.vs_closed_rtmfp();
					break;
			}
		}
		
		/**
		 * 
		 */
		private function event_rtmfp_vs(event:NetStatusEvent):void
		{
			log("[event_rtmfp_vs]..."+event.info.code+"\n");
		}
		/**
		 * 
		 */
		public function publishrtmfpoutstream(outgoingName:String,cbject:Object):Boolean
		{
			if( !_open || !_conn  ){
				return false;
			}
			try
			{
				_outstream = new NetStream(_conn, NetStream.DIRECT_CONNECTIONS);
				_outstream.addEventListener(NetStatusEvent.NET_STATUS, event_rtmfp_vs);
				_outstream.publish(outgoingName);
				_outstream.client = cbject;
			}
			catch (e:Error)
			{
				if( _outstream ){
					_outstream.close();
					_outstream.removeEventListener(NetStatusEvent.NET_STATUS, event_rtmfp_vs);
				}
				_outstream = null;
				return false;
			}
			return true;
		}
		
		/**
		 * 
		 */
		public function playrtmfpinstream(farId:String,incomingName:String,volumeValue:Number,cbject:Object):Boolean
		{
			if( !_open || !_conn  ){
				return false;
			}
			try
			{
				_instream = new NetStream(_conn, farId);
				_instream.addEventListener(NetStatusEvent.NET_STATUS, event_rtmfp_vs);
				_instream.client = cbject;
				_instream.play(incomingName);
				// set volume for incoming stream
				var st:SoundTransform = new SoundTransform(volumeValue);
				_instream.soundTransform = st;
				_instream.receiveAudio(true);
				_instream.receiveVideo(true);
			}
			catch (e:Error)
			{
				if( _instream ){
					_instream.close();
					_instream.removeEventListener(NetStatusEvent.NET_STATUS, event_rtmfp_vs);
				}
				_instream = null;
				return false;
			}
			return true;
		}
	}
	
}