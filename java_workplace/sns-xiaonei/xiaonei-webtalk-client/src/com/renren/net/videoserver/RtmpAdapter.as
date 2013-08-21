package com.renren.net.videoserver
{
	import com.renren.module.LocalItem;
	import com.renren.module.LocalStatus;
	import com.renren.module.Vctrl;
	import com.renren.net.videoserver.VideoAdapter;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.NetStatusEvent;
	import flash.external.ExternalInterface;
	import flash.media.Camera;
	import flash.media.Microphone;
	import flash.media.SoundTransform;
	import flash.net.NetConnection;
	import flash.net.NetStream;
	import flash.net.NetStreamInfo;
	import flash.net.Responder;
	
	import mx.core.FlexGlobals;
	public class RtmpAdapter extends VideoAdapter
	{
		/**
		 */
		[Event(name="vc_rtmp", type="com.renren.net.videoserver.VideoConnectEvent")]
		
		public function RtmpAdapter(vctrl:Vctrl,localstatus:LocalStatus,localitem:LocalItem)
		{
			super(vctrl,localstatus,localitem);
			this.addEventListener("vc_rtmp", this.vc_event_handler);
		}
		
		override protected function vc_event_handler(e:Event):void
		{
			var event:VideoConnectEvent = e as VideoConnectEvent;
			try
			{ 
				_conn.connect(videourl);
				_conn.call("idManager.add", null, localitem.userid); //向red5上增加此id
				log("[addID_to_red5]"+localitem.userid+"\n");
				FlexGlobals.topLevelApplication.bandWidthDetect(videourl);

			}
			catch (e:ArgumentError)
			{
				_conn.close();
				_conn = null;
				event.conn_fail_func();
			}
		}
		
		override public function vc():void
		{
			if( !_open || _conn ){
				vctrl.vc_rtmp_fail();
			}
			
			_conn = new NetConnection();
			_conn.addEventListener(NetStatusEvent.NET_STATUS, event_rtmp_vc);
			
			var d:VideoConnectEvent = new VideoConnectEvent("vc_rtmp",vctrl.vc_rtmp_fail );
			dispatchEvent(d);

			vctrl.vc_timer_start(); //开始计时
		}
		
		/**
		 * 
		 */
		private function event_rtmp_vc(event:NetStatusEvent):void
		{
			log("[event_rtmp_vc]handler_"+event.info.code+"\n");
			switch (event.info.code)
			{
				case "NetConnection.Connect.Success":
					vctrl.vc_rtmp_succ();
					break;
				
				case "NetConnection.Connect.Failed":
					vctrl.vc_rtmp_fail();
					break;
			}
		}
		
		/**
		 * 
		 */
		private function event_rtmp_vs(event:NetStatusEvent):void
		{
			log("[event_rtmp_vs]..."+event.info.code+"\n");
			switch (event.info.code)
			{
				case "NetStream.Play.PublishNotify":
					vctrl.vs_succ_rtmp();					
					break;
				
				case "NetStream.Play.UnpublishNotify":
					vctrl.vs_closed_rtmp();
					break;
				
				case "NetConnection.Connect.Closed":
					_conn.call("idManager.remove", null, localitem.userid); //从red5上 删除此 id
					log("[removeID_from_red5]"+localitem.userid+"\n");
					break;
			}
		}
		public function checkID_from_red5(caller_userid:String,getIsConnectedInfo:Function):void
		{
			var responder:Responder = new Responder(getIsConnectedInfo);
			_conn.call("idManager.isConnected", responder, caller_userid);//检查red5上是否有此id
			log("[checkID_from_red5]"+caller_userid+"\n");
		}

		public function publishrtmpoutstream(outgoingName:String):Boolean
		{
			if( !_open || !_conn ){
				return false;
			}
			try
			{
				_outstream = new NetStream(_conn, NetStream.CONNECT_TO_FMS);
				_outstream.addEventListener(NetStatusEvent.NET_STATUS, event_rtmp_vs);
				_outstream.publish(outgoingName);
				
			}
			catch (e:Error)
			{
				if( _outstream ){
					_outstream.close();
				}
				_outstream = null;
				return false;
			}
			return true;
		}
		
		public function playrtmpinstream(incomingName:String,volumeValue:Number,cbject:Object):Boolean
		{
			if( !_open || !_conn ){
				return false;
			}
			try
			{
				_instream = new NetStream(_conn,NetStream.CONNECT_TO_FMS);
				//指定在开始显示流之前需要多长时间将消息存入缓冲区
				_instream.bufferTime = 0.5; //设定 0.5 秒钟的时间缓冲区;
				_instream.addEventListener(NetStatusEvent.NET_STATUS, event_rtmp_vs);
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
					_instream.removeEventListener(NetStatusEvent.NET_STATUS, event_rtmp_vs);
				}
				_instream = null;
				return false;
			}
			return true;
		}
	}
	
}