package xce.tripod.monitor;

import redis.clients.jedis.Client;

public class RedisCacheObject implements TripodMonitoredObject {
	private Client client = null;
	private String clientString = null;

	public RedisCacheObject(String clientString) {
		this.clientString = clientString;
	}

	@Override
	public void init() {
		String[] units = clientString.split(":");
		if (units.length == 3) {
			String address = units[1];
			int port = 0;
			try {
				port = Integer.parseInt(units[2]);
			} catch (Exception e) {
				e.printStackTrace();
			}
			client = new Client(address, port);
		}
	}

	public void cleanUp() {
	}

	@Override
	public boolean isAlive() {
		try {
			client.ping();
			return true;
		} catch (Exception e) {
			return false;
		}
	}

	@Override
	public String getDesc() {
		String name = "RedisCache " + clientString;
		return name;

	}

}
