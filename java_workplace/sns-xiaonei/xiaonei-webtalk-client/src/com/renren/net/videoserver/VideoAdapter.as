package com.renren.net.videoserver
{
	import com.renren.module.LocalStatus;
	import com.renren.module.Vctrl;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.NetStatusEvent;
	import flash.media.Camera;
	import flash.media.Microphone;
	import flash.media.SoundTransform;
	import flash.net.NetConnection;
	import flash.net.NetStream;
	import flash.net.NetStreamInfo;
	import com.renren.module.LocalItem;
	import mx.core.FlexGlobals;
	
	public class VideoAdapter extends EventDispatcher
	{
		protected var vctrl:Vctrl;
		protected var localitem:LocalItem;
		protected var localstatus:LocalStatus;
		
		// xn video server address
		[Bindable] protected var videourl:String;
		
		protected var _open:Boolean;
		
		// this is the connection to xn video server
		protected var _conn:NetConnection;
		
		// outgoing media stream (audio, video, text and some control messages)
		protected var _outstream:NetStream;
		// incoming media stream (audio, video, text and some control messages)
		protected var _instream:NetStream;
		
		public function VideoAdapter(vctrl:Vctrl,localstatus:LocalStatus,localitem:LocalItem)
		{
			_open = false;
			this.vctrl = vctrl;
			this.localstatus = localstatus;
			this.localitem = localitem;
		}
		
		public function vc():void{};
		
		public function open(videourl:String):void
		{
			_open = true;
			this.videourl=videourl;
		}
		
		public function geturl():String
		{
			if( !_open ){
				return "0";
			}
			return videourl;
		}
		
		public function getinstreaminfo():NetStreamInfo
		{
			if (_open && _instream)
			{
				return _instream.info;
			}
			else
			{
				return null;
			}
		}
		public function getoutstreaminfo():NetStreamInfo
		{
			if (_open && _outstream)
			{
				return _outstream.peerStreams[0].info;
			}
			else
			{
				return null;
			}
		}
		
		public function close():void
		{
			_open = false;
			closestreams();
			
			if(_conn)
			{
				_conn.close();
			}
			_conn = null;
		}
		
		private function closestreams():void
		{
			if (_instream)
			{
				_instream.close();
			}
			_instream = null;
			if (_outstream)
			{
				_outstream.close();
			}
			_outstream = null;
		}
		
		public function attachoutaudio(mic:Microphone):void
		{
			if (_open && _outstream)
			{
				_outstream.attachAudio(mic);
			}
		}
		
		public function attachoutcamera(camera:Camera):void
		{
			if (_open && _outstream)
			{
				_outstream.attachCamera(camera);
			}
		}
		
		public function playaudio(av:Boolean):void
		{
			if (_open && _instream)
			{
				_instream.receiveAudio(av);
			}
		}
		
		public function playvideo(av:Boolean):void
		{
			if (_open && _instream)
			{
				_instream.receiveVideo(av);
			}
		}
		public function instreamsound(st:SoundTransform):void
		{
			if (_open && _instream)
			{
				_instream.soundTransform = st;
			}
		}
		public function sendim(name:String,msg:String):Boolean
		{
			if (_open && _outstream && msg.length!=0)
			{
				_outstream.send("onIm", name, msg);
				return true;
			}
			return false;
		}
		
		public function get instream():NetStream
		{
			return _instream;
		}
		
		public function get outstream():NetStream
		{
			return _outstream;
		}
		
		protected function vc_event_handler(e:Event):void{};
		
		protected function log(msg:String):void{
			FlexGlobals.topLevelApplication.log(msg);
		}
		
	}
}