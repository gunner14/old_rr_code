package org.red5.flash.bwcheck.app
{
	import com.renren.module.BwCheck;
	import flash.events.NetStatusEvent;
	import flash.external.ExternalInterface;
	import flash.net.NetConnection;
	import flash.net.Responder;
	
	import mx.controls.TextArea;
	import mx.core.Application;
	import mx.core.FlexGlobals;
	
	import org.red5.flash.bwcheck.ClientServerBandwidth;
	import org.red5.flash.bwcheck.ServerClientBandwidth;
	import org.red5.flash.bwcheck.events.BandwidthDetectEvent;
	
	public class BandwidthDetectionApp extends Application
	{
		private var _serverURL:String = "localhost";
		private var _serverApplication:String = "";
		private var _clientServerService:String = "";
		private var _serverClientService:String = "";		
		private var nc:NetConnection;
		private var oneOfRtmp:String="";
		private var bwcheck:BwCheck;
		
		public function BandwidthDetectionApp(bwcheck:BwCheck)
		{
			this.bwcheck = bwcheck;
		}
		
		public function set serverURL(url:String):void
		{
			_serverURL = url;
		}
		
		public function set serverApplication(app:String):void
		{
			_serverApplication = app;
		}
		
		public function set clientServerService(service:String):void
		{
			_clientServerService = service;
		}
		
		public function set serverClientService(service:String):void
		{
			_serverClientService = service;
		}
		
		public function connect(connectUrl:String):void
		{
			nc = new NetConnection();
			nc.objectEncoding = flash.net.ObjectEncoding.AMF0;
			nc.client = this;
			nc.addEventListener(NetStatusEvent.NET_STATUS, onStatus);				
			connectUrl = connectUrl.replace("live","bwCheck");
			nc.connect(connectUrl);
			oneOfRtmp=connectUrl;
		}
		
		
		private function onStatus(event:NetStatusEvent):void
		{
			switch (event.info.code)
			{
				case "NetConnection.Connect.Success":
					ServerClient();
				break;	
			}
			
		}
		
		public function ClientServer():void
		{
			var clientServer:ClientServerBandwidth  = new ClientServerBandwidth();
			clientServer.connection = nc;
			clientServer.service = _clientServerService;
			clientServer.addEventListener(BandwidthDetectEvent.DETECT_COMPLETE,onClientServerComplete);
			clientServer.addEventListener(BandwidthDetectEvent.DETECT_STATUS,onClientServerStatus);
			clientServer.addEventListener(BandwidthDetectEvent.DETECT_FAILED,onDetectFailed);
			clientServer.start();
		}
		
		public function ServerClient():void
		{
			var serverClient:ServerClientBandwidth = new ServerClientBandwidth();
			serverClient.connection = nc;
			serverClient.service = _serverClientService;
			serverClient.addEventListener(BandwidthDetectEvent.DETECT_COMPLETE,onServerClientComplete);
			serverClient.addEventListener(BandwidthDetectEvent.DETECT_STATUS,onServerClientStatus);
			serverClient.addEventListener(BandwidthDetectEvent.DETECT_FAILED,onDetectFailed);
			
			serverClient.start();
		}
		
		public function onDetectFailed(event:BandwidthDetectEvent):void
		{
			log("[bwCheck]Detection failed with error: " + event.info.application + " " + event.info.description+"\n");
		}
		
		public function onClientServerComplete(event:BandwidthDetectEvent):void
		{			
			log("[bwCheck]CS_kbitUp_" + event.info.kbitUp + "_deltaUp_" + event.info.deltaUp + "_deltaTime_" + event.info.deltaTime + "_latency_" + event.info.latency + "_KBytes_" + event.info.KBytes+"\n");
			bwcheck.setCSBW(event.info.KBytes); //带宽检测完成后抛出事件
		}
		
		public function onClientServerStatus(event:BandwidthDetectEvent):void
		{
			if (event.info) {
				//log("count: "+event.info.count+ " sent: "+event.info.sent+" timePassed: "+event.info.timePassed+" latency: "+event.info.latency+" overhead:  "+event.info.overhead+" packet interval: " + event.info.pakInterval + " cumLatency: " + event.info.cumLatency+"\n");
			}
		}
		
		public function onServerClientComplete(event:BandwidthDetectEvent):void
		{

			log("[bwCheck]SC_kbitDown_" + event.info.kbitDown + "_DeltaDown_" + event.info.deltaDown + "_DeltaTime_" + event.info.deltaTime + " Latency_" + event.info.latency+ "_KBytes_" +  event.info.kbitDown/8+"\n");
			bwcheck.setSCBW( event.info.kbitDown/8);
			ClientServer();
		}
		
		public function onServerClientStatus(event:BandwidthDetectEvent):void
		{	
			if (event.info) {
			   // log("\n count: "+event.info.count+ " sent: "+event.info.sent+" timePassed: "+event.info.timePassed+" latency: "+event.info.latency+" cumLatency: " + event.info.cumLatency);
			}
		}
		
		private function log(msg:String):void{
			FlexGlobals.topLevelApplication.log(msg);
		}

	}
}
