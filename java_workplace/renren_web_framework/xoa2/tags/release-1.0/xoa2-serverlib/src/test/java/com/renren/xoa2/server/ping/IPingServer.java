package com.renren.xoa2.server.ping;

import com.renren.xoa2.annotation.XoaService;

@XoaService("ping.xoa.renren.com")
public interface IPingServer extends PingServer.Iface {
	public int ping();
}
