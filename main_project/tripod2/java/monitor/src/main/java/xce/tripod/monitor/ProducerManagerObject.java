package xce.tripod.monitor;

import xce.tripod.ProducerManagerPrx;
import xce.tripod.ProducerManagerPrxHelper;
import Ice.Communicator;

/**
 * <p>
 * Title: ProducerManagerObject.java
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-5-29
 */
public class ProducerManagerObject implements TripodMonitoredObject {
	private ProducerManagerPrx proxy = null;
	private String proxyString = null;

	public ProducerManagerObject(String proxyString) {
		this.proxyString = proxyString;
	}

	@Override
	public void init() {
		String[] units = proxyString.split(":");
		if (units.length == 2) {
			String address = units[0];
			int port = 0;
			try {
				port = Integer.parseInt(units[1]);
			} catch (Exception e) {
				e.printStackTrace();
			}
			StringBuffer proxyStringBuf = new StringBuffer();
			proxyStringBuf.append("M:tcp -h ").append(address).append(" -p ").append(port).append(" -t ")
					.append(" 1000");
			Communicator communicator = Ice.Util.initialize();
			proxy = ProducerManagerPrxHelper.uncheckedCast(communicator.stringToProxy(proxyStringBuf.toString()));
		}
	}

	public void cleanUp() {
		if (proxy != null) {
			Communicator communicator = proxy.ice_getCommunicator();
			if (communicator != null) {
				try {
					communicator.destroy();
				} catch (Exception e) {
					System.err.println(e.getMessage());
				}
			}
		}
	}

	@Override
	public boolean isAlive() {
		if (proxy == null)
			return false;
		
		try {
			String result = proxy.ping();
			if (result.equalsIgnoreCase("PONG")) {
				return true;
			} else {
				return false;
			}
		} catch (Exception e) {
			return false;
		}
	}

	@Override
	public String getDesc() {
		String name = "ProducerManager " + proxyString;
		return name;

	}
}
